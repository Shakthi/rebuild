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
    
    
    
    statements.push_back(answer);
    
    

}

void ForStepProcessor::ExecuteLoop()
{
    for (forValue =  forValue +thisForBlock.forStep;
         forValue <thisForBlock.forEnd; forValue += thisForBlock.forStep) {
        
        
    }
    
}



