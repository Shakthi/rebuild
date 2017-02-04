//
//  BasicStepProccessor.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 04/02/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "BasicStepProccessor.hpp"
#include "Rebuild.hpp"
#include "lineNoiseWrapper.hpp"
#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include "quickbasic.h"


class ReadStepProcessor:public StepProcessor
{
    varTableIteratorList & list;
public:
    ReadStepProcessor(Rebuild * aRebuild,varTableIteratorList & alist)
    :StepProcessor(aRebuild),list(alist)
    {
        list = alist;
    }
    
    void RunStep()
    {
        
            //create the prompt for reading
            std::string readprompt = "[rebuild>input ";
            bool firstParam = true;
            for (auto i=list.begin() ; i!=list.end(); i++) {
                
                if(!firstParam)
                    readprompt += " ";
                
                auto aVarTableiter = *i;
                std::string varName = aVarTableiter->first;
                readprompt += varName;
                
                firstParam = false;
            }
            readprompt += "]:";
            
            
        std::string answer = rebuild->lineNoiseWrapper->getLine(readprompt);
        std::istringstream stream(answer);

        
        for (auto i=list.begin() ; i!=list.end(); i++) {
            
            auto aVarTableiter = *i;
            Value value = aVarTableiter->second;
            
            if (value.valutype == Value::Evaluetype::floattype) {
                
                stream>>value.numVal;
                std::string varname =  aVarTableiter->first;
                varTable[varname] = value;
                          
            }else if (value.valutype == Value::Evaluetype::stringtype){
                
                stream>>value.stringVal;
                std::string varname =  aVarTableiter->first;
                varTable[varname] = value;
            }
            
            list.pop_front();
            if (stream.eof()) {
                break;
            }
            
            
        }

        
        
        if(answer == ""){
            list.clear();
            exitProcessing();
        }
        
        if (list.empty()) {

            exitProcessing();

        }
        

        
            
            
            
        
        
        
    }
};



typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern int yyparse();
extern YY_BUFFER_STATE yy_scan_string(const char * str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);


void BasicStepProcessor::RunStep()
{
    
    std::string answer=rebuild->lineNoiseWrapper->getLine("[rebuild]:");
    
    
    if(answer=="") {
        
        //exitProcessing();
    }else{
        
        //parse the input
        yy_scan_string(answer.c_str());
        yyparse();
        
        if (!varReadList.empty()) {
        
        ReadStepProcessor * readStepProcessor =  new ReadStepProcessor(rebuild,varReadList);
        rebuild->addNewProcessing(readStepProcessor);
       
        
        }
    }
    
    
}
