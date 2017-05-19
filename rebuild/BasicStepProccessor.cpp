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

#include "linenoise/lineNoiseWrapper.hpp"
#include "Parser/ParserWrapper.hpp"
#include "Value.h"
#include "Logger.hpp"
#include "SentenceHistory.hpp"



#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include "ReadStepProcessor.hpp"







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
        
        ReadStepProcessor* readStepProcessor = new ReadStepProcessor(rebuild, readStatemnt->variableList,readStatemnt->prompt,localVarTable);
        rebuild->addNewProcessing(readStepProcessor);
        return true;
    }
    
    
    
    
    
    
    auto forStatemnt = dynamic_cast< ForStatment*>(result);
    if (forStatemnt) {
        
        if(!forStatemnt->statements.empty())
        {
        
            ForStepProcessor * processor = new ForStepProcessor(rebuild,forStatemnt,
                                                                &localVarTable);
            processor->Init();
            processor->ExecuteStatments(forStatemnt);

        }
        else
        {
            
            ForStepProcessor * readStepProcessor = new ForStepProcessor(rebuild,forStatemnt,&localVarTable);
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
        auto  * stepprocessor = new IfStepProcessor(rebuild,ifstment,&localVarTable);
        
        stepprocessor->Init();
        
        
        rebuild->addNewProcessing(stepprocessor);
        return true;
    }
    

    
    auto printStatemnt = dynamic_cast< PrintStatement*>(result);
    if (printStatemnt) {
        
        for (auto i= printStatemnt->printitems.begin();i!=printStatemnt->printitems.end();i++) {
            
            Value value=(*i)->Evaluate(&localVarTable);
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
        if(localVarTable.GetVar(letStatemnt->variablename).valutype == Value::Evaluetype::emptyType)
            localVarTable.GetVar(letStatemnt->variablename)=letStatemnt->rvalue->Evaluate(&localVarTable);
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
                    auto forStepProcessor = new ForStepProcessor(rebuild,forStatment,&localVarTable,ForStepProcessor::InitType::stepin);
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

        sentence= sentence->clone();

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
    j["localvarablelist"]=localVarTable.ToJson();

    return j;
}
void BasicStepProcessor::FromJson(nlohmann::json j)
{
    localVarTable.FromJson(j["localvarablelist"]);
}





