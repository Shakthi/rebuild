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
        LineNoiseWrapper::ExtraResults results;
        std::string answer = rebuild->lineNoiseWrapper->getLineWithHistory("[rebuild>if]:",stackedSentenceHistory,results);
        
        BasicParser parser;
        SentenceRef  result =  parser.Parse(answer);

        auto nextStatemnt = std::dynamic_pointer_cast< NextStatement>(result);
        if (nextStatemnt) {
            ExecuteHistory();
            exitProcessing();
            return;
        }
        
        auto errorStatemnt = std::dynamic_pointer_cast< ErrorStatement>(result);
        if (errorStatemnt) {
            BasicStepProcessor::Evaluate(errorStatemnt);
            stackedSentenceHistory.Add(errorStatemnt);
            return;
            
            
        }
        auto statemnt = std::dynamic_pointer_cast< Statement>(result);
        

        if(BasicStepProcessor::Evaluate(statemnt).addtoHistory)
            stackedSentenceHistory.Add(result);
        
        
    }else
    {
        std::string answer = rebuild->lineNoiseWrapper->getLine("[rebuild>if (remarks)]:");
        
        BasicParser parser;
        SentenceRef result =  parser.Parse(answer);
        
        auto remarkStatement = std::dynamic_pointer_cast< RemarkStatement>(result);
        if (remarkStatement) {
            remarks=remarkStatement->comments;
            exitProcessing();
            return;
        }
        
        
        
        exitProcessing();
        return;
    }
    
}

void IfStepProcessor::ExecuteHistory()
{
    
    
    assert(false);//TODO
//    for(  const auto & statement :stackedSentenceHistory.GetHistory() )
//    {
//        
//        Evaluate(dynamic_cast<Statement*>(statement.get()));
//    }

    
    
}