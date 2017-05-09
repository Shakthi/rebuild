//
//  Rebuild.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 31/01/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "Rebuild.hpp"
#include "Logger.hpp"

#include "BasicStepProccessor.hpp"
#include "linenoise/lineNoiseWrapper.hpp"
#include "StatementHistory.hpp"
#include <exception>
#include <stdexcept>
#include <iostream>

namespace {
    
    
    
    

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

bool replace(std::string& str, const std::string& from, const std::string& to)
{
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}
    
    
    
    
}


log_buffer lbuffer;
Rlog  rlog(lbuffer);



std::string Rebuild::prompt;

Rebuild::Rebuild()
    : exitStatus(exitStatusRIP)
    , alive(true)
{
    Rebuild::prompt="rebuild]";
    rlog << "Hello world...!("<<__DATE__<<"-"<<__TIME__<< ")"<<std::endl;
    history = new StatementHistory();
    lineNoiseWrapper = new LineNoiseWrapper(*history);
    processorStack.push(new BasicStepProcessor(this));
    Load();
}

std::string
Rebuild::LocalSavePath()
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
    }
}

namespace
{

    void Logger_FromJson(nlohmann::json options)
    {
        if(!options.is_null())
        {
            lbuffer.quit=options["logquite"];
        }
    }
    
    
    nlohmann::json Logger_ToJson()
    {
        nlohmann::json options;
        options["logquite"]=lbuffer.quit;
        return options;
    }



}



void Rebuild::Load()
{
    //Rlog rlog;
    std::string savepath = GetSavePath();
    
    rlog << "Loading main config:" << savepath << std::endl;
    std::ifstream stream(savepath);
    
    
    
    

    
    if (stream.good()) {
        nlohmann::json root;
        try {
            stream >> root;
            if (root["version"] != version)
                throw version;

            history->FromJson(root["history"]);
            processorStack.top()->FromJson(root["processor"]);
            Logger_FromJson(root["options"]);
            

            // successfull read lets save backup
            std::ofstream stream2(LocalSavePath());
            rlog << "Successfull load config..Backing up db at :" << LocalSavePath() << std::endl;

            stream2 << root.dump(4);

        } catch (std::exception exception) {
            
            
            rlog << Rlog::type::error<< "Failed load config"<< LocalSavePath() << exception.what() << std::endl;
            
        }
    }else
    {
         rlog << Rlog::type::error<< "Failed load config"<< LocalSavePath() << std::endl;
    
    }
}

void Rebuild::Save()
{
    //return;
    nlohmann::json root;
    //Rlog rlog;

    root["history"] = history->ToJson();

    root["timestampepoch"] = std::time(0);
    root["timestamp"] = nowTime();
    root["type"] = "rebuildalldb";
    root["creator"] = "rebuild";
    root["version"] = version;
    root["options"] = Logger_ToJson();

    root["processor"] = lastStepProcessorData;

    std::ofstream stream(GetSavePath());
    rlog << "Saving at "<<GetSavePath()<<"... " << std::endl;

    stream << root.dump(4);
}

Rebuild::~Rebuild()
{
    SaveIfLatest();
    rlog << "Goodbye world!... " <<std::endl;
    delete history;
    delete lineNoiseWrapper;
}

void Rebuild::exitProcessing()
{
    StepProcessor* last = processorStack.top();
    processorStack.pop();
    lastStepProcessorData = last->ToJson();
    delete last;
}

void Rebuild::addNewProcessing(StepProcessor* stepProcessor)
{
    processorStack.push(stepProcessor);
}

void Rebuild::RunStep()
{
    if (processorStack.empty()) {
        alive = false;
        return;
    }

    StepProcessor* stepProcessor = processorStack.top();

    stepProcessor->RunStep();
}
