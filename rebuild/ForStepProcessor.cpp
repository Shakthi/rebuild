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
    std::string answer = rebuild->lineNoiseWrapper->getLine("[rebuild>for "+ thisForBlock.forVar+"]:");
    
    BasicParser parser;
    Statement * result =  parser.Parse(answer);
    
    auto nextStatemnt = dynamic_cast< NextStatement*>(result);
    if (nextStatemnt) {
        delete result;
        ExecuteLoop();
        return;
    }
    
    auto errorStatemnt = dynamic_cast< ErrorStatement*>(result);
    if (errorStatemnt) {
        
        return;
    }
    
    if(BasicStepProcessor::Evaluate(result))
        statements.push_back(answer);
    
    

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



