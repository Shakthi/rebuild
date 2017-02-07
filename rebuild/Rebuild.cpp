//
//  Rebuild.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 31/01/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "Rebuild.hpp"

#include "lineNoiseWrapper.hpp"
#include "BasicStepProccessor.hpp"

#include <iostream>



namespace  {
    
    std::string nowTime()
    {
        time_t now;
        time(&now);
        char buf[sizeof "2011-10-08T07:07:09Z"];
        strftime(buf, sizeof buf, "%FT%TZ", gmtime(&now));
        return buf;
    }
    
    
    std::string version = "1.0";
    
    
    bool replace(std::string& str, const std::string& from, const std::string& to) {
        size_t start_pos = str.find(from);
        if(start_pos == std::string::npos)
            return false;
        str.replace(start_pos, from.length(), to);
        return true;
    }
    
    
    
}



Rebuild::Rebuild():exitStatus(exitStatusRIP),alive(true)
{
    lineNoiseWrapper = new LineNoiseWrapper();
    processorStack.push(new BasicStepProcessor(this));
    Load();
    
    
}

std::string Rebuild::LocalSavePath()
{
    
    const char *localPth = __FILE__;
    std::string original = localPth;
    replace(original,"rebuild/Rebuild.cpp","");
    return original+"rebuild.alldb.txt";
}



std::string Rebuild::GetSavePath()
{
    
    
    
    const char *homedir = getenv("HOME");
    return std::string("")+ homedir+"/.rebuild.alldb.txt";
}

void Rebuild::SaveIfLatest()
{
    std::string savepath = GetSavePath();
    
    std::ifstream stream(savepath);
    if (stream.good()) {
        nlohmann::json root;
        try
        {
            stream>>root;
        }
        catch (...)
        {
            
        }
        
        if(root["timestampepoch"].is_number())
        {   long int  timestampepoch = root["timestampepoch"];
            if (timestampepoch < std::time(0)) {
                
                Save();
            }
        }else
        {
            Save();
        }
        
        
    }
    
}

void Rebuild::Load()
{
    
    std::string savepath = GetSavePath();
    std::clog<<"Loading..."<<savepath<<std::endl;
    
    
    std::ifstream stream(savepath);
    if (stream.good()) {
        
        nlohmann::json root;
        try
        {
            stream>>root;
            if(root["version"] != version)
                throw version;
            
            
            lineNoiseWrapper->FromJson(root["history"]);
            processorStack.top()->FromJson(root["processor"]);
            
            //successfull read lets save backup
            std::ofstream stream2(LocalSavePath());
            stream2<<root.dump(4);
            
            
            
        } catch ( ...)
        {
            
        }
        
        
        
        
        
    }
    
    
    
    
    
    
}

void Rebuild::Save()
{
    std::clog<<"Saving..."<<std::endl;
    nlohmann::json root;
    
    root["history"]=lineNoiseWrapper->ToJson();
    
    root["timestampepoch"]=std::time(0);
    root["timestamp"]=nowTime();
    root["type"]="rebuildalldb";
    root["creator"]="rebuild";
    root["version"]=version;
    
    
    root["processor"]=lastStepProcessorData;
    
    std::ofstream stream(GetSavePath());
    stream<<root.dump(4);
    
    
    
    
}



Rebuild::~Rebuild()
{
    SaveIfLatest();
    delete lineNoiseWrapper;
    
}


void Rebuild::exitProcessing()
{
    StepProcessor * last = processorStack.top();
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
    
    
    StepProcessor  * stepProcessor = processorStack.top();
    
    stepProcessor->RunStep();
    
    
}

