//
//  BasicStepProccessor.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 04/02/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "BasicStepProccessor.hpp"
#include "ForStepProcessor.hpp"
#include "Rebuild.hpp"
#include "lineNoiseWrapper.hpp"
#include "ParserWrapper.hpp"
#include "quickbasic.h"
#include "Logger.hpp"

#include <iostream>
#include <map>
#include <sstream>
#include <vector>

class ReadStepProcessor : public StepProcessor {
    std::list<std::string> list;
    std::string prompt;

public:
    ReadStepProcessor(Rebuild* aRebuild, std::list<std::string>& alist,std::string aPrompt)
        : StepProcessor(aRebuild),prompt(aPrompt)
        , list(alist)
    {
        list = alist;
    }

    nlohmann::json ToJson()
    {
        nlohmann::json j;
        return j;
    }

    void FromJson(nlohmann::json j) {}

    void RunStep()
    {
        std::string readprompt;
        
        
        if(prompt!="")
        {
            
            readprompt=Rebuild::prompt+prompt+"";
        }
        
        else
        {
        // create the prompt for reading
        readprompt = "[rebuild>input ";
        bool firstParam = true;
        for (auto i = list.begin(); i != list.end(); i++) {
            if (!firstParam)
                readprompt += " ";

            
            std::string varName = *i;
            readprompt += varName;

            firstParam = false;
        }
        readprompt += "]:";
        }

        std::string answer = rebuild->lineNoiseWrapper->getLine(readprompt);
        std::istringstream stream(answer);

        for (auto i = list.begin(); i != list.end(); i++) {
            auto variableName = *i;
            
            if(variableName.find("$") == std::string::npos){ //hack
                Value value;
                stream >> value.numVal;
                varTable[variableName] = value;
                rlog<<variableName <<"="<<value.numVal<<std::endl;

            } else if(variableName.find("$") != std::string::npos){
                Value value;
                stream >> value.stringVal;
                varTable[variableName] = value;
                rlog<<variableName <<"="<<value.stringVal<<std::endl;
            }

            list.pop_front();
            if (stream.eof()) {
                break;
            }
        }

        if (answer == "") {
            list.clear();
            exitProcessing();
        }

        if (list.empty()) {
            exitProcessing();
        }
    }
};









nlohmann::json
BasicStepProcessor::ToJson()
{
    nlohmann::json j;

    for (auto varaible : varTable) {
        if (varaible.second.valutype == Value::Evaluetype::floattype) {
            nlohmann::json item;
            item["key"] = varaible.first;
            item["value"] = varaible.second.numVal;
            j.push_back(item);
        } else if (varaible.second.valutype == Value::Evaluetype::stringtype) {
            nlohmann::json item;
            item["key"] = varaible.first;
            item["value"] = varaible.second.stringVal;
            j.push_back(item);
        }
    }

    nlohmann::json basicStep;
    basicStep["varablelist"] = j;

    return basicStep;
}

void BasicStepProcessor::FromJson(nlohmann::json j)
{
    nlohmann::json varablelist = j["varablelist"];
    for (nlohmann::json item : varablelist) {
        if (item["value"].is_number()) {
            Value value;
            value.valutype = Value::Evaluetype::floattype;
            value.numVal = item["value"];
            varTable[item["key"]] = value;

        } else if (item["value"].is_string()) {
            Value value;
            value.valutype = Value::Evaluetype::stringtype;
            value.stringVal = item["value"];
            varTable[item["key"]] = value;
        }
    }
}

bool BasicStepProcessor::Evaluate(Statement  * result)
{
    
    
    auto endStatement = dynamic_cast< EndStatement*>(result);
    if (endStatement) {
        exitProcessing();
        delete result;
        return false;
    }
    
    auto readStatemnt = dynamic_cast< ReadStatement*>(result);
    if (readStatemnt) {
        
        ReadStepProcessor* readStepProcessor = new ReadStepProcessor(rebuild, readStatemnt->variableList,readStatemnt->prompt);
        rebuild->addNewProcessing(readStepProcessor);
        delete result;
        return true;
    }
    
    auto forStatemnt = dynamic_cast< ForStatment*>(result);
    if (forStatemnt) {
        
        ForStepProcessor * readStepProcessor = new ForStepProcessor(rebuild, forStatemnt->forBlock);
        rebuild->addNewProcessing(readStepProcessor);
        delete result;
        return true;
    }
    

    return true;
}

void BasicStepProcessor::RunStep()
{
    std::string answer = rebuild->lineNoiseWrapper->getLine(Rebuild::prompt);
    

    
    if (answer == ""){
        if( rebuild->lineNoiseWrapper->GetStatus() == LineNoiseWrapper::EStatus::ctrl_c)
        exitProcessing();
        return;
    }

    
    BasicParser parser;
    
    
    Evaluate(parser.Parse(answer));
    
        
            
    
}



