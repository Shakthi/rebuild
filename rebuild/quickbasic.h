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
    
    float getNumVal()const
    {
        assert(Evaluetype::floattype == valutype);
        return numVal;
    }
    
    std::string getStringVal()const
    {
        assert(Evaluetype::stringtype == valutype);
        return stringVal;
    }
    
    bool getBoolVal()const
    {
        assert(Evaluetype::booltype == valutype);
        return boolVal;
    }

    
    Value(float aNumVall):numVal(aNumVall){ valutype = Evaluetype::floattype;}
    
    Value(std::string aStringVall):stringVal(aStringVall){valutype = Evaluetype::stringtype;}
    Value(bool aboolval):boolVal(aboolval){valutype = Evaluetype::booltype;}
    
    
    
    Value(class Expression * aExpression):expression(aExpression){valutype = Evaluetype::othertype;}
    Value(class Sentence * anodeVal):nodeVal(anodeVal){valutype = Evaluetype::othertype;}


    Value(){ valutype = Evaluetype::emptyType;}
    
    
    
    union
    {
        class Sentence * nodeVal;
        class Expression * expression;
    };

    static const Value & nullValue;
    
    
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

extern bool parserQuits;


#define register 


#endif /* quickbasic_h */
