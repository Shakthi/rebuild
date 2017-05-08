//
//  ParserWrapper.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 13/02/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "ParserWrapper.hpp"
#include "quickbasic.h"





typedef struct yy_buffer_state* YY_BUFFER_STATE;
extern int yyparse(Sentence**);
extern YY_BUFFER_STATE yy_scan_string(const char* str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);



 Sentence * Parser::Parse(std::string data)
{
    yy_scan_string(data.c_str());
    Sentence * outSentence;
    if(yyparse(&outSentence)==0){
        if( outSentence != NULL)
            outSentence->sourceText = data;
        return  outSentence;
        
    } else {
        if(outSentence==nullptr)
            return new ErrorStatement("");
        else{
            outSentence->sourceText = data;
            return  outSentence;
        }
        
    }
    
    
}



