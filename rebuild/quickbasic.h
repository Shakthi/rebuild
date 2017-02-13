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
#include "AST.hpp"
struct Value
{
    enum class Evaluetype
    {
        emptyType,
        stringtype,
        booltype,
        floattype,
        inttype,
        chartype,
        nodeType
        
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
    Statement * statement;
    
    Value():valutype(Evaluetype::emptyType){}
    ~Value(){ if(valutype == Evaluetype::nodeType ) delete (statement); }
    
};



struct ForBlock
{
    std::string forVar;
    float  forBegin;
    float  forEnd;
    
    float  forStep;
    
    bool foundNext;
    
    
};


#define YYSTYPE struct Value
typedef std::list<std::map<std::string,Value>::iterator> varTableIteratorList;

extern std::map<std::string,Value> varTable;
extern std::list<ForBlock> forBlockList;
extern varTableIteratorList varReadList;
;

extern bool parserQuits;





#endif /* quickbasic_h */
