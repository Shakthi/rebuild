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


void ForStepProcessor::Init()
{
    
    varTable[thisForBlock.forVar] = thisForBlock.forBegin->Evaluate();
    if(getForVar() <=thisForBlock.forEnd->Evaluate().getNumVal() ) {
             passThrough=true;
    }

    


}


void ForStepProcessor::RunStep()
{
    
    if(passThrough == true){
    
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
        
    
    }else 
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

    
    for (varTable[thisForBlock.forVar] =  getForVar() +thisForBlock.forStep->Evaluate().getNumVal() ;
         getForVar() <=thisForBlock.forEnd->Evaluate().getNumVal() ; varTable[thisForBlock.forVar]= getForVar() +thisForBlock.forStep->Evaluate().getNumVal() ) {
        
        for( std::string statement :popingLineHistory.GetHistory() )
        {
            BasicParser parser;
            Statement * result =  parser.Parse(statement);

            Evaluate(result);
        }
        
    }
    
}



