//
//  ForStepProcessor.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 14/02/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "ForStepProcessor.hpp"
#include "linenoise/lineNoiseWrapper.hpp"
#include "Parser/ParserWrapper.hpp"


void ForStepProcessor::Init()
{
    
    varTable[thisForBlock->forVar] = thisForBlock->forBegin->Evaluate();
    if(getForVar() <=thisForBlock->forEnd->Evaluate().getNumVal() ) {
             passThrough=true;
    }
}




 
void ForStepProcessor::RunStep()
{
    
    if(passThrough == true){
    
    std::string answer = rebuild->lineNoiseWrapper->getLineWithHistory("[rebuild>for "+ thisForBlock->forVar+"]:",popingLineHistory);
    
    BasicParser parser;
    Statement * result =  parser.Parse(answer);
    popingLineHistory.PopExtra();

    auto nextStatemnt = dynamic_cast< NextStatement*>(result);
    if (nextStatemnt) {
        delete result;
        ExecuteHistory();
        
        std::vector<Statement*> v{ std::make_move_iterator(std::begin(popingLineHistory.GetModifieableHistory())),
            std::make_move_iterator(std::end(popingLineHistory.GetModifieableHistory())) };
        
        
        thisForBlock->statements = std::move(v);
        
        
        
        exitProcessing();
        return;
    }
    
        
    auto endStatement = dynamic_cast< EndStatement*>(result);
    if (endStatement) {
        delete result;
        exitProcessing();
        return;
    }
        
    auto errorStatemnt = dynamic_cast< ErrorStatement*>(result);
    if (errorStatemnt) {
        BasicStepProcessor::Evaluate(errorStatemnt);
        popingLineHistory.AddExtra(errorStatemnt);
        return;


    }
        
        auto statemnt = dynamic_cast< ListStatement*>(result);
        if (statemnt) {
            
            std::cout<<std::endl;
            
            
            for (auto currentStatment : popingLineHistory.GetHistory()) {
                
                std::cout<<currentStatment->dumpToString()<<std::endl;
                
            }
            
            
            delete statemnt;
            return ;
        }

    
    if(BasicStepProcessor::Evaluate(result))
        popingLineHistory.Add(result);
        
    
    }else 
    {
        
        std::string answer = rebuild->lineNoiseWrapper->getLineWithHistory("[rebuild>forelse]:",popingLineHistory);

        BasicParser parser;
        Statement * result =  parser.Parse(answer);
        popingLineHistory.PopExtra();
        
        auto endStatement = dynamic_cast< EndStatement*>(result);
        if (endStatement) {
            delete result;
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

    }

}


void ForStepProcessor::ExecuteStatments(ForStatment  * thisForBlock)
{

    
    for (varTable[thisForBlock->forVar] =  varTable[thisForBlock->forVar].getNumVal() +thisForBlock->forStep->Evaluate().getNumVal() ;
         varTable[thisForBlock->forVar].getNumVal() <=thisForBlock->forEnd->Evaluate().getNumVal() ; varTable[thisForBlock->forVar]= varTable[thisForBlock->forVar].getNumVal() +thisForBlock->forStep->Evaluate().getNumVal() )
        
    {
        
        for (auto i=thisForBlock->statements.rbegin(); i != thisForBlock->statements.rend(); i++) {
            
            Evaluate((*i));
            
        }
        
    }
    


}

void ForStepProcessor::ExecuteHistory()
{
    popingLineHistory.PopExtra();

    
    for (varTable[thisForBlock->forVar] =  getForVar() +thisForBlock->forStep->Evaluate().getNumVal() ;
         getForVar() <=thisForBlock->forEnd->Evaluate().getNumVal() ; varTable[thisForBlock->forVar]= getForVar() +thisForBlock->forStep->Evaluate().getNumVal() ) {
        
        
        
        for (auto i=popingLineHistory.GetHistory().rbegin(); i != popingLineHistory.GetHistory().rend(); i++) {
            
            Evaluate((*i));
            
        }
        
        
        
    }
    
}



