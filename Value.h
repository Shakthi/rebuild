//
//  Value.h
//  rebuild
//
//  Created by Shakthi Prasad G S on 12/05/17.
//  Copyright © 2017 self. All rights reserved.
//

#ifndef Value_h
#define Value_h

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

#endif /* Value_h */
