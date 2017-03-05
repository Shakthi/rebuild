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

struct Value
{
    enum class Evaluetype
    {
        emptyType,
        stringtype,
        booltype,
        floattype
        
        
    };
    Evaluetype valutype;
    
    union
    {
        float   numVal;
        bool    boolVal;
        char    charVal;
        class Statement * nodeVal;
        class Expression * expression;
    };
    std::string stringVal;
    
    
       
};





#define YYSTYPE struct Value

extern std::map<std::string,Value> varTable;

extern bool parserQuits;


#define register 


#endif /* quickbasic_h */
