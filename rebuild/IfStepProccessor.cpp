//
//  IfStepProccessor.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 12/03/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "IfStepProccessor.hpp"
#include "lineNoiseWrapper.hpp"
#include "ParserWrapper.hpp"


void IfStepProcessor::Init()
{
    
    
    if(ifStatment->expression->Evaluate().getBoolVal() ) {
        passThroughe=true;
    }
    
    
    
    
}


void IfStepProcessor::RunStep()
{
    
    if(passThroughe == true){
        
        std::string answer = rebuild->lineNoiseWrapper->getLineWithHistory("[rebuild>if]:",popingLineHistory);
        
        BasicParser parser;
        Statement * result =  parser.Parse(answer);
        popingLineHistory.PopExtra();
        
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
            popingLineHistory.AddExtra(errorStatemnt);
            return;
            
            
        }
        
        if(BasicStepProcessor::Evaluate(result))
            popingLineHistory.Add(result);
        
        
    }else
    {
        std::string answer = rebuild->lineNoiseWrapper->getLine("[rebuild>if (remarks)]:");
        
        BasicParser parser;
        Statement * result =  parser.Parse(answer);
        
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
    popingLineHistory.PopExtra();
    
    
  
    for( auto statement :popingLineHistory.GetHistory() )
    {
        
        Evaluate(statement);
    }
        
    
    
}