//
//  BasicStepProccessor.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 04/02/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "BasicStepProccessor.hpp"
#include "ForStepProcessor.hpp"
#include "IfStepProccessor.hpp"
#include "Rebuild.hpp"
#include "lineNoiseWrapper.hpp"
#include "ParserWrapper.hpp"
#include "quickbasic.h"
#include "Logger.hpp"
#include "StatementHistory.hpp"



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
        readprompt = Rebuild::prompt +"input ";
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

        std::string answer = rebuild->lineNoiseWrapper->getLine(readprompt);
        std::istringstream stream(answer);

        for (auto i = list.begin(); i != list.end(); i++) {
            auto variableName = *i;
            
            if(variableName.find("$") == std::string::npos){ //hack
                
                float num;
                stream >> num;
                varTable[variableName] = Value(num);
                rlog<<variableName <<"="<<varTable[variableName].getNumVal()<<std::endl;

            } else if(variableName.find("$") != std::string::npos){
                Value value;
                std::string str;
                stream >> str;
                varTable[variableName] = Value(str);
                rlog<<variableName <<"="<<varTable[variableName].getStringVal()<<std::endl;
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
            item["value"] = varaible.second.getNumVal();
            j.push_back(item);
        } else if (varaible.second.valutype == Value::Evaluetype::stringtype) {
            nlohmann::json item;
            item["key"] = varaible.first;
            item["value"] = varaible.second.getStringVal();
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
            varTable[item["key"]] = Value((float)item["value"]);

        } else if (item["value"].is_string()) {
            
            varTable[item["key"]] = Value(item["value"].get<std::string>());
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
        
        ForStepProcessor * readStepProcessor = new ForStepProcessor(rebuild, std::move(forStatemnt->forBlock));
        readStepProcessor->Init();
        
        
        rebuild->addNewProcessing(readStepProcessor);
        delete result;
        return true;
    }
    
    auto errorStatemnt = dynamic_cast< ErrorStatement*>(result);
    if (errorStatemnt) {
        
        std::cout<<Rebuild::prompt<<"! "<<errorStatemnt->description<<std::endl;
        delete result;
        return false;
    }
    
    
    auto ifstment = dynamic_cast< IfStatment*>(result);
    if (ifstment) {
        auto  * stepprocessor = new IfStepProcessor(rebuild,ifstment->expression);
        
        stepprocessor->Init();
        
        
        rebuild->addNewProcessing(stepprocessor);
        delete result;
        return true;
    }
    

    
    auto printStatemnt = dynamic_cast< PrintStatement*>(result);
    if (printStatemnt) {
        
        for (auto i= printStatemnt->printitems.begin();i!=printStatemnt->printitems.end();i++) {
            
            Value value=(*i)->Evaluate();
            if(value.valutype == Value::Evaluetype::stringtype)
                std::cout<<value.getStringVal();
            else  if(value.valutype == Value::Evaluetype::floattype)
                std::cout<<value.getNumVal();
            else if(value.valutype == Value::Evaluetype::emptyType)
                std::cout<<"<NULL>";
            else  if(value.valutype == Value::Evaluetype::booltype)
                std::cout<<(value.getBoolVal()?"true":"false");
            

        }
        std::cout<<std::endl;
        
        delete result;
        return true;
    }
    
    auto letStatemnt = dynamic_cast< LetStatement*>(result);
    if (letStatemnt) {
        
        varTable[letStatemnt->variablename]=letStatemnt->rvalue->Evaluate();
        delete result;
        return false;
    }
    
    
    

    return true;
}

void BasicStepProcessor::RunStep()
{
    std::string answer = rebuild->lineNoiseWrapper->getLine(Rebuild::prompt+":");
    

    
    if (answer == ""){
        if( rebuild->lineNoiseWrapper->GetStatus() == LineNoiseWrapper::EStatus::ctrl_c)
        exitProcessing();
        return;
    }
    
    rebuild->history->Add(answer);
    

    
    BasicParser parser;
    Evaluate(parser.Parse(answer));
    
}



