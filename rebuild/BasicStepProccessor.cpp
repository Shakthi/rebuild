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
#include "Sentence.h"
#include "Logger.hpp"
#include "SentenceHistory.hpp"



#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include "ReadStepProcessor.hpp"







BasicStepProcessor::CmdResult BasicStepProcessor::Evaluate(StatementRef   result)
{
    CmdResult ret{true,true};

    if (result==nullptr) {
        ret.addtoHistory = false;
        ret.handled = false;
        return ret;
    }
    
    auto endStatement = std::dynamic_pointer_cast < EndStatement>(result);
    if (endStatement) {
        exitProcessing();
        ret.addtoHistory = false;
        ret.handled = true;
        return ret;

    }
    
    auto readStatemnt = std::dynamic_pointer_cast < ReadStatement>(result);
    if (readStatemnt) {
        
        auto readStepProcessor = std::shared_ptr<StepProcessor>(new ReadStepProcessor(rebuild, readStatemnt->variableList,readStatemnt->prompt,localVarTable));
        rebuild->addNewProcessing(readStepProcessor);
        return ret; //TODO:need to add to history later
    }
    
    
    
    
    
    
    auto forStatemnt = std::dynamic_pointer_cast< ForStatment>(result);
    if (forStatemnt) {
        
        if(!forStatemnt->statements.empty())
        {
        
            ForStepProcessor * processor = new ForStepProcessor(rebuild,forStatemnt,
                                                                &localVarTable,ForStepProcessor::InitType::reload);
            processor->Init();
           processor->ExecuteStatments(forStatemnt);

            delete processor;
            

        }
        else
        {
            
            auto readStepProcessor =  new ForStepProcessor(rebuild,forStatemnt,&localVarTable);
            readStepProcessor->Init();
            
            
            rebuild->addNewProcessing(std::shared_ptr<StepProcessor>(readStepProcessor));

        
        }
            
        return ret; //TODO:need to add to history later
    }
    
    auto errorStatemnt = std::dynamic_pointer_cast< ErrorStatement>(result);
    if (errorStatemnt) {
        
        std::cout<<Rebuild::prompt<<"! "<<errorStatemnt->description<<std::endl;
        return ret;//TODO and as temporary
    }
    
    
    auto ifstment = std::dynamic_pointer_cast< IfStatment>(result);
    if (ifstment) {
        auto  * stepprocessor = new IfStepProcessor(rebuild,ifstment,&localVarTable);
        
        stepprocessor->Init();
        
        
        rebuild->addNewProcessing(std::shared_ptr<StepProcessor>(stepprocessor));
        return ret; //TODO:need to add to history later
    }
    

    
    auto printStatemnt = std::dynamic_pointer_cast< PrintStatement>(result);
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
        if( ! std::dynamic_pointer_cast< PrintElementStatement>(result))
            std::cout<<std::endl;
        
        return ret;
    }

   
    
    
    auto letStatemnt = std::dynamic_pointer_cast< LetStatement>(result);
    if (letStatemnt) {
        if(localVarTable.GetVar(letStatemnt->variablename).valutype == Value::Evaluetype::emptyType)
            localVarTable.GetVar(letStatemnt->variablename)=letStatemnt->rvalue->Evaluate(&localVarTable);
        else
            std::cout<<Rebuild::prompt<<"! "<<letStatemnt->variablename<<" already defined"<<std::endl;


        return ret;
    }
    
    
    

    return ret;
}






 BasicStepProcessor::CmdResult BasicStepProcessor::Process(std::shared_ptr<Command> result)
{

    CmdResult ret{true,true};
    if (result==nullptr) {
        ret.addtoHistory = false;
        ret.handled = false;
        return ret;

    }

    
    {
        
       
        auto statemnt = std::dynamic_pointer_cast< ListCommand>(result);
        if (statemnt) {
            
            std::cout<<std::endl;
            int count=1;
            for (auto i=history->rcbegin(); i!=history->rcend(); i++) {
                std::cout<<count<<" "<<(*i)->dumpToString()<<std::endl;
                count++;
            }
            
            
            
            return ret;
        }
        auto customCommand = std::dynamic_pointer_cast<CustomCommand>(result);
        if (customCommand) {
            
            if(customCommand->name == "restart")
            {
                rebuild->restart();
            
            }
            else if (customCommand->name == "cls")
            {
                
                rebuild->lineNoiseWrapper->ClearScreen();
                return ret;

            }
            else if (customCommand->name == "stepin")
            {
                if (history->GetLastStatmentIter()!= history->end()) {
                    
                    SentenceRef sentence = *(history->GetLastStatmentIter());
                    auto  forStatment =  std::dynamic_pointer_cast<ForStatment>(sentence);
                    if(forStatment)
                    {
                        auto forStepProcessor = new ForStepProcessor(rebuild,forStatment,&localVarTable,ForStepProcessor::InitType::stepin);
                        forStepProcessor->Init();

                        rebuild->addNewProcessing(std::shared_ptr<StepProcessor>(forStepProcessor));
                    }

                }

                
                return ret;
                
            }
            else
            {   rlog<<Rlog::type::error<<"Not found command"<<customCommand->name<<std::endl;
                return ret;//Need to add temporarray
            }
            
        }
        

    }

    ret.addtoHistory = false;
    ret.handled = false;
    return ret;


}


std::string BasicStepProcessor::ProcessCtrlKeyStroke(int ctrlchar)
{
    Rlog rlog;
    switch (ctrlchar+'a'-1) {
        case 'z':return ".restart";
            break;
        case 'x':return ".stepin";
            break;

        case 's':return ".checkback";
            break;



        default:   rlog<<"\n"<<std::string("key")+char('a'+ctrlchar-1)<<std::endl;
            return std::string("key")+char('a'+ctrlchar-1);
            break;
    }
    
}

void BasicStepProcessor::RunStep()
{
    LineNoiseWrapper::ExtraResults results;
    std::string answer = rebuild->lineNoiseWrapper->getLineWithHistory(Rebuild::prompt + ":",*rebuild->history,results);

    bool proceedStroke = false;

    if (answer == "") {
        if (results.status == LineNoiseWrapper::ExitStatus::ctrl_c) {
            exitProcessing();
            return;
        }

        if (results.status == LineNoiseWrapper::ExitStatus::ctrl_X) {
            proceedStroke = true;
            answer = ProcessCtrlKeyStroke(results.ctrlKey);
        }
    }

    SentenceRef sentence;
    LineNoiseWrapper::EModificationStatus mstats = results.mstatus;

    if (results.status == LineNoiseWrapper::ExitStatus::ok && mstats == LineNoiseWrapper::EModificationStatus::history) {

        auto lastiter = history->GetLastStatmentIter();

        assert(*lastiter);
        sentence = SentenceRef((*lastiter)->clone());

    } else {
        BasicParser parser;
        sentence = parser.Parse(answer);
    }

    assert(sentence);

    auto result = Evaluate(std::dynamic_pointer_cast<Statement>(sentence));

    if (result.handled) {

        if (result.addtoHistory)
            history->Add(sentence);

    } else {
        auto result = Process(std::dynamic_pointer_cast<Command>(sentence));

        if (result.handled) {

            if (!proceedStroke && result.addtoHistory) {
                history->Add(sentence);
            }
        }
    }
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





