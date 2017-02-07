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

#define YYSTYPE struct Value
typedef std::list<std::map<std::string,Value>::iterator> varTableIteratorList;
extern std::map<std::string,Value> varTable;
extern varTableIteratorList varReadList;
;

extern bool parserQuits;

struct ForBlock
{
    std::string forVar;
    float  forBegin;
    float  forEnd;
    
    float  forStep;
    
 
};





#endif /* quickbasic_h */
