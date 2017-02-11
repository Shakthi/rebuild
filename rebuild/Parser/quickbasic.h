//
//  quickbasic.h
//  rebuild
//
//  Created by Shakthi Prasad G S on 04/02/17.
//  Copyright © 2017 self. All rights reserved.
//

#ifndef quickbasic_h
#define quickbasic_h
#include<list>
#include<map>
struct Value
{
    enum class Evaluetype
    {
        stringtype,
        booltype,
        floattype,
        inttype,
        chartype,
        
    };
    Evaluetype valutype;
    
    union
    {
        float   numVal;
        int     intVal;
        bool    boolVal;
        char    charVal;
    };
    std::string stringVal;
};



struct ForBlock
{
    std::string forVar;
    float  forBegin;
    float  forEnd;
    
    float  forStep;
    
    bool foundNext;
    
    
};


typedef std::list<std::map<std::string,Value>::iterator> varTableIteratorList;

extern std::map<std::string,Value> varTable;
extern std::list<ForBlock> forBlockList;
extern varTableIteratorList varReadList;
;

extern bool parserQuits;


#define YYSTYPE struct Value
#undef YY_DECL
#define YY_DECL					\
yy::parser::token_type                         \
yy::basicscanner::yylex (yy::parser::semantic_type * yylval,      \
yy::parser::location_type* yylloc)
// ... and declare it for the parser's sake.




#endif /* quickbasic_h */
