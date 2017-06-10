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

        auto nextStatemnt = dynamic_cast< NextStatement*>(result);
        if (nextStatemnt) {
            delete result;
            ExecuteHistory();
            
            thisForBlock->statements.clear();
            for (auto i =popingLineHistory.rbegin(); i !=popingLineHistory.rend();i++) {
                
                auto statement = dynamic_cast<Statement*>(*i);
                if(statement)
                {
                    thisForBlock->statements.push_back(statement->clone());
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
            popingLineHistory.Add(errorStatemnt);
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
                
                auto endStatement = dynamic_cast< EndStatement*>(result);
                if (endStatement) {
                    delete result;
                    exitProcessing();
                    return;
                }
                
                auto errorStatemnt = dynamic_cast< ErrorStatement*>(result);
                if (errorStatemnt) {
                    BasicStepProcessor::Evaluate(errorStatemnt);
                    popingLineHistory.Add(errorStatemnt);
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
            int count=1,tabstop=0;

            auto historyStackIter  =rebuild->GetHistoryStack().begin();
            auto historyStackIterEnd  =rebuild->GetHistoryStack().end();


            while (historyStackIter!=historyStackIterEnd) {

                for (auto i= (*historyStackIter)->rbegin(); i!=(*historyStackIter)->rend(); i++) {

                    std::cout<<count;

                    for(int i=0;i<tabstop+1;i++)
                        std::cout<<" ";
                    std::cout<<(*i)->dumpToString()<<std::endl;
                    count++;
                }
                historyStackIter++; tabstop++;
            }


            
            return true;
        }

        
        auto customCommand = dynamic_cast<CustomCommand*>(input);
        if (customCommand) {
            
            if (customCommand->name == "popback")
            {
                    popingLineHistory.PopHistory();
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
            
            
            
            for (auto i=popingLineHistory.rbegin(); i != popingLineHistory.rend(); i++) {
                
                Evaluate(dynamic_cast<Statement*>((*i)));
                
            }
            
            once=false   ;

        }
     }

    void ForStepProcessor::ExecuteHistory()
    {
        
        
        for (localVarTable.GetVar(thisForBlock->forVar) =  getForVar() +thisForBlock->forStep->Evaluate(&localVarTable).getNumVal() ;
             getForVar() <=thisForBlock->forEnd->Evaluate(&localVarTable).getNumVal() ; localVarTable.GetVar(thisForBlock->forVar)= getForVar() +thisForBlock->forStep->Evaluate(&localVarTable).getNumVal() ) {
            
            
            
            for (auto i=popingLineHistory.rbegin(); i != popingLineHistory.rend(); i++) {
                
                Evaluate(dynamic_cast<Statement*>((*i)));
                
            }
            
            
            
        }
        
    }
    
    
    
