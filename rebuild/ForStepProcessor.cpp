//
//  ForStepProcessor.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 14/02/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "ForStepProcessor.hpp"
#include "lineNoiseWrapper.hpp"
#include "ParserWrapper.hpp"





void ForStepProcessor::RunStep()
{
    
    if(getForVar() <=thisForBlock.forEnd->Evaluate().numVal ) {
    
    isInited =true;
    std::string answer = rebuild->lineNoiseWrapper->getLineWithHistory("[rebuild>for "+ thisForBlock.forVar+"]:",popingLineHistory);
    
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
        
    
    }else if(!isInited)
    {
        std::string answer = rebuild->lineNoiseWrapper->getLine("[rebuild>for "+ thisForBlock.forVar+" (remarks)]:");
        
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

void ForStepProcessor::ExecuteLoop()
{
    popingLineHistory.PopExtra();

    
    for (getForVar() =  getForVar() +thisForBlock.forStep->Evaluate().numVal ;
         getForVar() <=thisForBlock.forEnd->Evaluate().numVal ; getForVar() += thisForBlock.forStep->Evaluate().numVal ) {
        
        for( std::string statement :popingLineHistory.GetHistory() )
        {
            BasicParser parser;
            Statement * result =  parser.Parse(statement);

            Evaluate(result);
        }
        
    }
    
}



