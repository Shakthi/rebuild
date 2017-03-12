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
    
    
    if(expresion->Evaluate().getBoolVal() ) {
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
            ExecuteLoop();
            exitProcessing();
            return;
        }
        
        auto errorStatemnt = dynamic_cast< ErrorStatement*>(result);
        if (errorStatemnt) {
            BasicStepProcessor::Evaluate(errorStatemnt);
            popingLineHistory.AddExtra(answer);
            return;
            
            
        }
        
        if(BasicStepProcessor::Evaluate(result))
            popingLineHistory.Add(answer);
        
        
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

void IfStepProcessor::ExecuteLoop()
{
    popingLineHistory.PopExtra();
    
    
  
        for( std::string statement :popingLineHistory.GetHistory() )
        {
            BasicParser parser;
            Statement * result =  parser.Parse(statement);
            
            Evaluate(result);
        }
        
    
    
}