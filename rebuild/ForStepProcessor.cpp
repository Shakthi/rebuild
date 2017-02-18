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
    
    if(getForVar() <=thisForBlock.forEnd ) {
    
    isInited =true;
    
    std::string answer = rebuild->lineNoiseWrapper->getLine("[rebuild>for "+ thisForBlock.forVar+"]:");
    
    BasicParser parser;
    Statement * result =  parser.Parse(answer);
    
    auto nextStatemnt = dynamic_cast< NextStatement*>(result);
    if (nextStatemnt) {
        delete result;
        ExecuteLoop();
        exitProcessing();
        return;
    }
    
    auto errorStatemnt = dynamic_cast< ErrorStatement*>(result);
    if (errorStatemnt) {
        
        return;
    }
    
    if(BasicStepProcessor::Evaluate(result))
        statements.push_back(answer);
    
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
    for (getForVar() =  getForVar() +thisForBlock.forStep;
         getForVar() <=thisForBlock.forEnd; getForVar() += thisForBlock.forStep) {
        
        for( std::string statement :statements )
        {
            BasicParser parser;
            Statement * result =  parser.Parse(statement);

            Evaluate(result);
        }
        
    }
    
}



