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
    
    localVarTable.GetVar(thisForBlock->forVar) = thisForBlock->forBegin->Evaluate(&localVarTable);
    if(getForVar() <=thisForBlock->forEnd->Evaluate(&localVarTable).getNumVal() ) {
        passThrough=true;
    }
}





void ForStepProcessor::RunStep()
{
    
    if(passThrough == true){
        
        std::string answer = rebuild->lineNoiseWrapper->getLineWithHistory(Rebuild::prompt+"for "+ thisForBlock->forVar+"]:",popingLineHistory);
        
        BasicParser parser;
        Sentence * result =  parser.Parse(answer);
        popingLineHistory.PopExtra();
        
        auto nextStatemnt = dynamic_cast< NextStatement*>(result);
        if (nextStatemnt) {
            delete result;
            ExecuteHistory();
            
            thisForBlock->statements.clear();
            for (auto i =popingLineHistory.GetHistory().rbegin(); i !=popingLineHistory.GetHistory().rend();) {
                
                auto statement = dynamic_cast<Statement*>(*i);
                if(statement)
                {
                    thisForBlock->statements.push_back(statement->clone());
                }


                    i++;
                
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
        //localVarTable.SetSuper(&outer);
        
        
        for (localVarTable.GetVar(thisForBlock->forVar) =  localVarTable.GetVar(thisForBlock->forVar).getNumVal();
             localVarTable.GetValue(thisForBlock->forVar).getNumVal() <=thisForBlock->forEnd->Evaluate(&localVarTable).getNumVal() ; localVarTable.GetVar(thisForBlock->forVar)= localVarTable.GetValue(thisForBlock->forVar).getNumVal() +thisForBlock->forStep->Evaluate(&localVarTable).getNumVal() )
            
        {
            
            for (auto i=thisForBlock->statements.begin(); i != thisForBlock->statements.end(); i++) {
                
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
            for (auto i= popingLineHistory.GetHistory().rbegin(); i!=popingLineHistory.GetHistory().rend(); i++) {
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
                    popingLineHistory.PopHistory(popingLineHistory.GetHistory().begin());
                return true;
                
            }
            else if (customCommand->name == "nextstep")
            {   //This command  makes next iteration of loop
                //Ideally this should be behaviour of next statement
                ExecuteAStep();
                return true;
                
            }
            else
            {
                return BasicStepProcessor::Process(input);
            }
        }
        
        return false;
            

    }


    void ForStepProcessor::ExecuteAStep()
    {
        bool once=true;
        for ( ;getForVar() <=thisForBlock->forEnd->Evaluate(&localVarTable).getNumVal() && once; localVarTable.GetVar(thisForBlock->forVar)= getForVar() +thisForBlock->forStep->Evaluate(&localVarTable).getNumVal() ) {
            
            
            
            for (auto i=popingLineHistory.GetHistory().rbegin(); i != popingLineHistory.GetHistory().rend(); i++) {
                
                Evaluate(dynamic_cast<Statement*>((*i)));
                
            }
            
            once=false   ;

        }
     }

    void ForStepProcessor::ExecuteHistory()
    {
        popingLineHistory.PopExtra();
        
        
        for (localVarTable.GetVar(thisForBlock->forVar) =  getForVar() +thisForBlock->forStep->Evaluate(&localVarTable).getNumVal() ;
             getForVar() <=thisForBlock->forEnd->Evaluate(&localVarTable).getNumVal() ; localVarTable.GetVar(thisForBlock->forVar)= getForVar() +thisForBlock->forStep->Evaluate(&localVarTable).getNumVal() ) {
            
            
            
            for (auto i=popingLineHistory.GetHistory().rbegin(); i != popingLineHistory.GetHistory().rend(); i++) {
                
                Evaluate(dynamic_cast<Statement*>((*i)));
                
            }
            
            
            
        }
        
    }
    
    
    
