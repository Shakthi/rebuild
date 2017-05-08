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
#include "SentenceHistory.hpp"



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
};








bool BasicStepProcessor::Evaluate(Statement  * result)
{
    if (result==nullptr) {
        return false;
    }
    
    auto endStatement = dynamic_cast< EndStatement*>(result);
    if (endStatement) {
        exitProcessing();
        return true;
    }
    
    auto readStatemnt = dynamic_cast< ReadStatement*>(result);
    if (readStatemnt) {
        
        ReadStepProcessor* readStepProcessor = new ReadStepProcessor(rebuild, readStatemnt->variableList,readStatemnt->prompt);
        rebuild->addNewProcessing(readStepProcessor);
        return true;
    }
    
    
    
    
    
    
    auto forStatemnt = dynamic_cast< ForStatment*>(result);
    if (forStatemnt) {
        
        if(!forStatemnt->statements.empty())
        {
        
            ForStepProcessor * processor = new ForStepProcessor(rebuild,forStatemnt);
            processor->Init();
            processor->ExecuteStatments(forStatemnt);

        }
        else
        {
            
            ForStepProcessor * readStepProcessor = new ForStepProcessor(rebuild,forStatemnt);
            readStepProcessor->Init();
            
            rebuild->addNewProcessing(readStepProcessor);

        
        }
            
        return true;
    }
    
    auto errorStatemnt = dynamic_cast< ErrorStatement*>(result);
    if (errorStatemnt) {
        
        std::cout<<Rebuild::prompt<<"! "<<errorStatemnt->description<<std::endl;
        return true;
    }
    
    
    auto ifstment = dynamic_cast< IfStatment*>(result);
    if (ifstment) {
        auto  * stepprocessor = new IfStepProcessor(rebuild,ifstment);
        
        stepprocessor->Init();
        
        
        rebuild->addNewProcessing(stepprocessor);
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
        if( ! dynamic_cast< PrintElementStatement*>(result))
            std::cout<<std::endl;
        
        return true;
    }

   
    
    
    auto letStatemnt = dynamic_cast< LetStatement*>(result);
    if (letStatemnt) {
        if(varTable.GetVar(letStatemnt->variablename).valutype == Value::Evaluetype::emptyType)
            varTable.GetVar(letStatemnt->variablename)=letStatemnt->rvalue->Evaluate();
        else
            std::cout<<Rebuild::prompt<<"! "<<letStatemnt->variablename<<" already defined"<<std::endl;


        return true;
    }
    
    
    

    return true;
}






bool BasicStepProcessor::Process(Command  * result)
{
    if (result==nullptr) {
        return false;
    }
    
    
    {
        
       
        auto statemnt = dynamic_cast< ListCommand*>(result);
        if (statemnt) {
            
            std::cout<<std::endl;
            int count=1;
            for (auto i=rebuild->history->GetHistory().rbegin(); i!=rebuild->history->GetHistory().rend(); i++) {
                std::cout<<count<<" "<<(*i)->dumpToString()<<std::endl;
                count++;
            }
            
            
            
            return true;
        }
        auto customCommand = dynamic_cast<CustomCommand*>(result);
        if (customCommand) {
            
            if(customCommand->name == "restart")
            {
                rebuild->restart();
            
            }
            else if (customCommand->name == "cls")
            {
                
                rebuild->lineNoiseWrapper->ClearScreen();
                return true;

            }
            else if (customCommand->name == "stepin")
            {
                Sentence * sentence = rebuild->history->GetCurrentStatment();
                auto  forStatment =  dynamic_cast<ForStatment*>(sentence);
                if(forStatment)
                {
                    auto forStepProcessor = new ForStepProcessor(rebuild,forStatment,
                                                                 ForStepProcessor::InitType::stepin);
                    forStepProcessor->Init();
                    
                    rebuild->addNewProcessing(forStepProcessor);

                    
                
                }
                
                
                return true;
                
            }
            else
            {   rlog<<Rlog::type::error<<"Not found command"<<customCommand->name<<std::endl;
                return true;
            }
            
        }
        

    }

    return false;

}


std::string BasicStepProcessor::ProcessCtrlKeyStroke(int ctrlchar)
{
    Rlog rlog;
    switch (ctrlchar+'a'-1) {
        case 'z':return ".restart";
            break;
        case 'x':return ".stepin";
            break;

            
        default:   rlog<<"\n"<<std::string("key")+char('a'+rebuild->lineNoiseWrapper->GetControlKey()-1)<<std::endl;
            return std::string("key")+char('a'+rebuild->lineNoiseWrapper->GetControlKey()-1);
            break;
    }
    
}

void BasicStepProcessor::RunStep()
{
    std::string answer = rebuild->lineNoiseWrapper->getLine(Rebuild::prompt+":");
    

    bool proceedStroke=false;

    if (answer == ""){
        if( rebuild->lineNoiseWrapper->GetStatus() == LineNoiseWrapper::ExitStatus::ctrl_c)
        {
            exitProcessing();
            return;
        
        }
        
        
        if( rebuild->lineNoiseWrapper->GetStatus() == LineNoiseWrapper::ExitStatus::ctrl_X)
        {
            proceedStroke=true;
            answer = ProcessCtrlKeyStroke(rebuild->lineNoiseWrapper->GetControlKey());
        
        }
        
        
    }
    
    Sentence * sentence;
    LineNoiseWrapper::EModificationStatus mstats = rebuild->lineNoiseWrapper->GetModificationStatus();
    
    
        
    if(mstats == LineNoiseWrapper::EModificationStatus::history)
    {
        sentence = rebuild->history->GetCurrentStatment();
    }
    else
    {
        BasicParser parser;
        sentence = parser.Parse(answer);
    }
    
    assert(sentence);

    if(Evaluate(dynamic_cast<Statement*>(sentence)))
    {
        rebuild->history->Add(sentence);

    }else if(Process(dynamic_cast<Command*>(sentence)))
    {
        if (!proceedStroke) {
            rebuild->history->Add(sentence);
        }
        
    }else assert(false && "Should not happan");
        
    
    
}


nlohmann::json BasicStepProcessor::ToJson()
{
    nlohmann::json j;
    j["varablelist"]=varTable.ToJson();

    return j;
}
void BasicStepProcessor::FromJson(nlohmann::json j)
{
    varTable.FromJson(j["varablelist"]);
}





