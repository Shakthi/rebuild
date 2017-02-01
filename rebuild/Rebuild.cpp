//
//  Rebuild.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 31/01/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "Rebuild.hpp"

#include "lineNoiseWrapper.hpp"

#include <iostream>


Rebuild::Rebuild():exitStatus(exitStatusRIP),alive(true)
{
    Load();
    lineNoiseWrapper = new LineNoiseWrapper();
}


void Rebuild::Load()
{
    std::clog<<"Loading..."<<std::endl;
}


Rebuild::~Rebuild()
{
    delete lineNoiseWrapper;

}

typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern int yyparse();
extern YY_BUFFER_STATE yy_scan_string(const char * str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);



void Rebuild::RunStep()
{
    
    
    
    std::string answer=lineNoiseWrapper->getLine("rebuild:");
    yy_scan_string(answer.c_str());
    yyparse();
    
    if(answer!="")
    {
        std::cout<<"rebuild>"<<answer<<std::endl;
    }
    else
        alive = false;
    

}

