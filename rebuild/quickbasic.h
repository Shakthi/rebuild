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
#include<memory>
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
    Statement * nodeVal;
    
       
};





#define YYSTYPE struct Value

extern std::map<std::string,Value> varTable;

extern bool parserQuits;





#endif /* quickbasic_h */