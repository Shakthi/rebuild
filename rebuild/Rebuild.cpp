//
//  Rebuild.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 31/01/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "Logger.hpp"
#include "Rebuild.hpp"

#include "BasicStepProccessor.hpp"
#include <exception>
#include <iostream>
#include <stdexcept>

namespace {
//Anonymised namespace for utility
    
//https://stackoverflow.com/questions/9527960/how-do-i-construct-an-iso-8601-datetime-in-c
std::string
nowTime()
{
    time_t now;
    time(&now);
    char buf[sizeof "2011-10-08T07:07:09Z"];
    strftime(buf, sizeof buf, "%FT%TZ", gmtime(&now));
    return buf;
}

std::string version = "1.0";

//https://stackoverflow.com/questions/3418231/replace-part-of-a-string-with-another-string
bool replace(std::string& str, const std::string& from, const std::string& to)
{
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}
}//


//Hirarchical loging facility
log_buffer lbuffer;
Rlog rlog(lbuffer);


Rebuild::Rebuild(const std::vector<std::string>& argv)
    : exitStatus(exitStatusRIP)
    , alive(true)
    , arglist(argv)
    , lineNoiseWrapper(history)

{
    //Rlog rlog;
    rlog << arglist.size() << std::endl;

    rlog << "Hello world...!(" << __DATE__ << "-" << __TIME__ << ")" << std::endl;


    InitProcessor();





}


Rebuild::~Rebuild()
{
    //Note no allocation
}

std::string
Rebuild::GetLocalSavePath()
{
    const char* localPth = __FILE__;
    std::string original = localPth;
    replace(original, "rebuild/Rebuild.cpp", "");
    return original + "rebuild.alldb.txt";
}

std::string
Rebuild::GetSavePath()
{
    const char* homedir = getenv("HOME");
    return std::string("") + homedir + "/.rebuild.alldb.txt";
}

void Rebuild::SaveIfLatest()
{
    std::string savepath = GetSavePath();

    std::ifstream stream(savepath);
    if (stream.good()) {
        nlohmann::json root;
        try {
            stream >> root;
        } catch (...) {
        }

        if (root["timestampepoch"].is_number()) {
            long int timestampepoch = root["timestampepoch"];
            if (timestampepoch < std::time(0)) {
                Save();
            }
        } else {
            Save();
        }
    } else {
        Save();
    }
}

namespace {

void Logger_FromJson(nlohmann::json options)
{
    if (!options.is_null()) {
        lbuffer.quit = options["logquite"];
    }
}

nlohmann::json Logger_ToJson()
{
    nlohmann::json options;
    options["logquite"] = lbuffer.quit;
    return options;
}
}

void Rebuild::Load()
{
    Rlog rlog;
    std::string savepath = GetSavePath();

    rlog << "Loading main config:" << savepath << std::endl;
    std::ifstream stream(savepath);

    if (stream.good()) {
        nlohmann::json root;
        try {
            stream >> root;
            if (root["version"] != version)
                throw version;
            varTable.FromJson(root["globalVariable"]);

            history.FromJson(root["history"]);
            processorStack.top()->FromJson(root["processor"]);
            Logger_FromJson(root["options"]);

            // successfull read lets save backup
            std::ofstream stream2(GetLocalSavePath());
            rlog << "Successfull load config..Backing up db at :" << GetLocalSavePath() << std::endl;

            stream2 << root.dump(4);

        } catch (std::exception exception) {

            rlog << Rlog::type::error << "Failed load config" << GetLocalSavePath() << exception.what() << std::endl;
        }
    } else {
        rlog << Rlog::type::error << "Failed load config" << GetLocalSavePath() << std::endl;
    }
}

void Rebuild::Save()
{
    //return;
    nlohmann::json root;
    //Rlog rlog;
    root["globalVariable"] = varTable.ToJson();
    root["history"] = history.ToJson();

    root["timestampepoch"] = std::time(0);
    root["timestamp"] = nowTime();
    root["type"] = "rebuildalldb";
    root["creator"] = "rebuild";
    root["version"] = version;
    root["options"] = Logger_ToJson();

    root["processor"] = lastStepProcessorData;

    std::ofstream stream(GetSavePath());
    rlog << "Saving at " << GetSavePath() << "... " << std::endl;

    stream << root.dump(4);
}


void Rebuild::ExitProcessing()
{

    if(!lastPromptList.empty())
        lastPromptList.pop_back();

    lastPromptString="";

    for (const std::string a: lastPromptList) {
        lastPromptString+=a;
    }

    auto last = processorStack.top();
    processorStack.pop();
    historyStack.pop_back();
    lastStepProcessorData = last->ToJson();
}

void Rebuild::AddNewProcessing(std::shared_ptr<StepProcessor> stepProcessor)
{
    if(!processorStack.empty())
        lastPromptList.push_back(processorStack.top()->GetPrompt());
    lastPromptString="";
    for (const std::string a: lastPromptList) {
        lastPromptString+=a;
    }

    processorStack.push(stepProcessor);
    historyStack.push_back(stepProcessor->GetHistory());
}


 const std::string Rebuild::GetPrompt() {

    
    return lastPromptString;


}

void Rebuild::Substep(const StepProcessor * stepProcessor)
{

    while(!IsTopStepProcessor(stepProcessor)) {

        RunStep();
    }

}






bool Rebuild::IsTopStepProcessor(const StepProcessor * process){

    auto stepProcessor = processorStack.top();

    return (stepProcessor.get() == process);
}

void Rebuild::RunStep()
{
    if (processorStack.empty()) {
        alive = false;
        return;
    }

    auto stepProcessor = processorStack.top();

    stepProcessor->RunStep();
}

#include <unistd.h>

bool Rebuild::Restart()
{
    //Note this is hard restart. Loads the rebuilt binary from the disk. Very usefull if the rebuild getting built.
    //Whith perfect implementation restore

    Rlog rlog;
    rlog << "Restarting " << std::endl;

    SaveIfLatest();
    char* argm[] = { strdup(arglist[0].c_str()), 0 };


    execvp(argm[0], argm);

    return true;
}