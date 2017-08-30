//
//  ReadStepProcessor.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 12/05/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "ReadStepProcessor.hpp"
#include "Rebuild.hpp"
#include "linenoise/lineNoiseWrapper.hpp"
#include "Logger.hpp"

#include "Value.h"

ReadStepProcessor::ReadStepProcessor(Rebuild* aRebuild, std::list<std::string>& alist,std::string aPrompt,VarTable & aVartable)
    : StepProcessor(aRebuild),prompt(aPrompt)
    , list(alist)
    ,varTable(aVartable)
    {
        list = alist;
    }

    nlohmann::json ReadStepProcessor::ToJson()
    {
        nlohmann::json j;
        return j;
    }

    void ReadStepProcessor::FromJson(nlohmann::json j) {}

    void ReadStepProcessor::RunStep()
    {
        std::string readprompt;


        if(prompt!="")
        {

            readprompt=rebuild->GetPrompt()+prompt+"";
        }

        else
        {
            // create the prompt for reading
            readprompt = rebuild->GetPrompt() +"input ";
            bool firstParam = true;
            for (auto i = list.begin(); i != list.end(); i++) {
                if (!firstParam)
                    readprompt += " ";

                readprompt += "\'";
                std::string varName = *i;
                readprompt += varName;
                readprompt += "\'";

                firstParam = false;
            }
            readprompt += ":";
        }

        std::string answer = rebuild->lineNoiseWrapper.getLine(readprompt);
        std::istringstream stream(answer);

        for (auto i = list.begin(); i != list.end(); i++) {
            auto variableName = *i;

            if(variableName.find("$") == std::string::npos){ //hack

                float num;
                stream >> num;
                varTable.GetVar(variableName) = Value(num);
                rlog<<variableName <<"="<<varTable.GetVar(variableName).getNumVal()<<std::endl;

            } else if(variableName.find("$") != std::string::npos){
                Value value;
                std::string str;
                stream >> str;
                varTable.GetVar(variableName) = Value(str);
                rlog<<variableName <<"="<<varTable.GetVar(variableName).getStringVal()<<std::endl;
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



