//
//  ParserWrapper.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 13/02/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "ParserWrapper.hpp"



typedef struct yy_buffer_state* YY_BUFFER_STATE;
extern int yyparse();
extern YY_BUFFER_STATE yy_scan_string(const char* str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);



Statement  Parser::Parse(std::string data)
{
    yy_scan_string(data.c_str());
    
    if(yyparse()){
        
        return Statement();
        
    } else {
        return ErrorStatement();
        
    }
    
    
    
        
    
    
    
    
}



