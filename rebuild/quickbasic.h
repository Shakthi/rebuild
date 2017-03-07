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
#include<assert.h>

struct Value
{
    enum class Evaluetype
    {
        emptyType,
        stringtype,
        booltype,
        floattype,
        othertype
        
    };
    Evaluetype valutype;
    
    float getNumVal()
    {
        assert(Evaluetype::floattype == valutype);
        return numVal;
    }
    
    std::string getStringVal()
    {
        assert(Evaluetype::stringtype == valutype);
        return stringVal;
    }
    
    Value(float aNumVall):numVal(aNumVall){ valutype = Evaluetype::floattype;}
    
    Value(std::string aStringVall):stringVal(aStringVall){valutype = Evaluetype::stringtype;}
    
    Value(class Expression * aExpression):expression(aExpression){valutype = Evaluetype::othertype;}
    Value(class Statement * anodeVal):nodeVal(anodeVal){valutype = Evaluetype::othertype;}


    Value(){}
    
    
    
    union
    {
        class Statement * nodeVal;
        class Expression * expression;
    };

    
private:
    union
    {
        float   numVal;
        bool    boolVal;
        char    charVal;
    };
    
    
    std::string stringVal;
    
    
       
};





#define YYSTYPE struct Value

extern std::map<std::string,Value> varTable;

extern bool parserQuits;


#define register 


#endif /* quickbasic_h */
