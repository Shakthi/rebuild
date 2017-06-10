//
//  IfStepProccessor.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 12/03/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "IfStepProccessor.hpp"
#include "linenoise/lineNoiseWrapper.hpp"
#include "Parser/ParserWrapper.hpp"


void IfStepProcessor::Init()
{
    
    
    if(ifStatment->expression->Evaluate(&localVarTable).getBoolVal() ) {
        passThroughe=true;
    }
    
    
    
    
}


void IfStepProcessor::RunStep()
{
    
    if(passThroughe == true){
        
        std::string answer = rebuild->lineNoiseWrapper->getLineWithHistory("[rebuild>if]:",popingLineHistory);
        
        BasicParser parser;
        Sentence * result =  parser.Parse(answer);

        auto nextStatemnt = dynamic_cast< NextStatement*>(result);
        if (nextStatemnt) {
            delete result;
            ExecuteHistory();
            exitProcessing();
            return;
        }
        
        auto errorStatemnt = dynamic_cast< ErrorStatement*>(result);
        if (errorStatemnt) {
            BasicStepProcessor::Evaluate(errorStatemnt);
            popingLineHistory.Add(errorStatemnt);
            return;
            
            
        }
        auto statemnt = dynamic_cast< Statement*>(result);
        

        if(BasicStepProcessor::Evaluate(statemnt))
            popingLineHistory.Add(result);
        
        
    }else
    {
        std::string answer = rebuild->lineNoiseWrapper->getLine("[rebuild>if (remarks)]:");
        
        BasicParser parser;
        Sentence * result =  parser.Parse(answer);
        
        auto remarkStatement = dynamic_cast< RemarkStatement*>(result);
        if (remarkStatement) {
            remarks=remarkStatement->comments;
            delete result;
            exitProcessing();
            return;
        }
        
        
        
        delete result;
        exitProcessing();
        return;
    }
    
}

void IfStepProcessor::ExecuteHistory()
{
    
    
    assert(false);//TODO
//    for(  const auto & statement :popingLineHistory.GetHistory() )
//    {
//        
//        Evaluate(dynamic_cast<Statement*>(statement.get()));
//    }

    
    
}