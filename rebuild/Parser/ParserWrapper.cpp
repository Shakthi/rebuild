//
//  ParserWrapper.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 13/02/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "ParserWrapper.hpp"
#include "Value.h"





typedef struct yy_buffer_state* YY_BUFFER_STATE;
extern int yyparse(Sentence**);
extern YY_BUFFER_STATE yy_scan_string(const char* str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
typedef void* yyscan_t;
extern void yylex_destroy();
extern void yylex_init ( ) ;
extern void yy_delete_buffer( YY_BUFFER_STATE buffer);



 SentenceRef Parser::Parse(std::string data)
{
    YY_BUFFER_STATE buf;
    buf = yy_scan_string(data.c_str());
    Sentence * outSentence;
    if(yyparse(&outSentence)==0){
        if( outSentence != NULL)
            outSentence->sourceText = data;
            yy_delete_buffer(buf);

            yylex_destroy();
        return  SentenceRef(outSentence);
        
    } else {
        if(outSentence==nullptr)
            return SentenceRef(new  ErrorStatement(""));
        else{
            outSentence->sourceText = data;
            return  SentenceRef(outSentence);
        }
        
    }
    
    
}



