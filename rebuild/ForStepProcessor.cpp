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
        Sentence * result =  parser.Parse(answer);
        popingLineHistory.PopExtra();
        
        auto nextStatemnt = dynamic_cast< NextStatement*>(result);
        if (nextStatemnt) {
            delete result;
            ExecuteHistory();
            
            thisForBlock->statements.clear();
            for (auto i =popingLineHistory.GetHistory().begin(); i !=popingLineHistory.GetHistory().end();) {
                
                auto statement = dynamic_cast<Statement*>(*i);
                if(statement)
                {
                    thisForBlock->statements.push_back(statement);
                    i=popingLineHistory.GetModifieableHistory().erase(i);
                }else
                {
                    i++;
                }
                
            }
            
            
            
            
            
            
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
               
        {
            auto statemnt = dynamic_cast< Statement*>(result);
            
            
            if(statemnt && BasicStepProcessor::Evaluate(statemnt))
                popingLineHistory.Add(result);
            else if(Process(dynamic_cast< Command*>(result)))
                ;
            else
            {
                
                std::string answer = rebuild->lineNoiseWrapper->getLineWithHistory("[rebuild>forelse]:",popingLineHistory);
                
                BasicParser parser;
                Sentence * result =  parser.Parse(answer);
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
                
                auto statemnt = dynamic_cast< Statement*>(result);
                
                
                if(BasicStepProcessor::Evaluate(statemnt))
                    popingLineHistory.Add(result);
                
            }
        }
        
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


    bool ForStepProcessor::Process( Command* input)
    {
        
        auto statemnt = dynamic_cast< ListCommand*>(input);
        if (statemnt) {
            
            std::cout<<std::endl;
            int count=1;
            for (auto i= popingLineHistory.GetHistory().begin(); i!=popingLineHistory.GetHistory().end(); i++) {
                std::cout<<count<<" "<<(*i)->dumpToString()<<std::endl;
                count++;
            }
            
            
            
            return true;
        }

        
        auto customCommand = dynamic_cast<CustomCommand*>(input);
        if (customCommand) {
            
            if (customCommand->name == "popback")
            {
                if(popingLineHistory.GetHistory().size())
                    popingLineHistory.GetModifieableHistory().pop_front();
                return true;
                
            }
            else
            {
                return BasicStepProcessor::Process(input);
            }
        }
        
        return false;
            

    }

    void ForStepProcessor::ExecuteHistory()
    {
        popingLineHistory.PopExtra();
        
        
        for (varTable[thisForBlock->forVar] =  getForVar() +thisForBlock->forStep->Evaluate().getNumVal() ;
             getForVar() <=thisForBlock->forEnd->Evaluate().getNumVal() ; varTable[thisForBlock->forVar]= getForVar() +thisForBlock->forStep->Evaluate().getNumVal() ) {
            
            
            
            for (auto i=popingLineHistory.GetHistory().rbegin(); i != popingLineHistory.GetHistory().rend(); i++) {
                
                Evaluate(dynamic_cast<Statement*>((*i)));
                
            }
            
            
            
        }
        
    }
    
    
    
