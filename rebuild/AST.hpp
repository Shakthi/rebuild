//
//  AST.hpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 13/02/17.
//  Copyright © 2017 self. All rights reserved.
//

#ifndef AST_hpp
#define AST_hpp
#include <memory>
#include <list>
#include <string>
#include <vector>
#include "quickbasic.h"


struct Statement {
    
    std::string sourceText;
    virtual ~Statement(){}
};


struct ErrorStatement:public Statement {
    std::string description;
    ErrorStatement(std::string aDescription):description(aDescription){}
public:
    
};



struct EndStatement:public  Statement{
};


struct NextStatement:public  Statement{
    
};



struct RemarkStatement:public  Statement{
    std::string comments;

};



struct ForStatment:public Statement {
    
    struct ForBlock
    {
        std::string forVar;
        std::unique_ptr<Expression> forBegin,forEnd,forStep;
        bool foundNext;
        
        ForBlock(ForBlock && inforblock):forVar(inforblock.forVar)
        {
            forBegin =std::move(inforblock.forBegin);
            forEnd =std::move(inforblock.forEnd);
            forStep =std::move(inforblock.forStep);
        }
        
        
        ForBlock()
        {
            
        }
        
        
        void Set(ForBlock && inforblock)
        {
            forVar =inforblock.forVar;
            forBegin =std::move(inforblock.forBegin);
            forEnd =std::move(inforblock.forEnd);
            forStep =std::move(inforblock.forStep);
        }
        
    } forBlock;
    

};




struct ReadStatement:public  Statement{
public:
    std::string prompt;
    std::list<std::string> variableList;
    
    
};




struct PrintStatement:public  Statement{
    
    
public:
    
    std::vector<Expression*> printitems;
    
}
;




struct Expression
{
    
    virtual Value Evaluate()=0;
};


struct ExpressionList:Expression
{
    std::vector<Expression*> list;
    Value Evaluate(){return Value();}
    
};




struct BainaryExpression :Expression
{
    Value::Evaluetype valuetype;
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
    
    Value Evaluate()
    {
        Value result;
        
        switch(valuetype)
        {
            case  Value::Evaluetype::stringtype:
                result.stringVal = left->Evaluate().stringVal + right->Evaluate().stringVal;
                return result;
                
        
        }
        
        result.valutype = Value::Evaluetype::floattype;


        switch(mOperator)
        {
            case operatorType::plus:
                result.numVal = left->Evaluate().numVal + right->Evaluate().numVal;
                return result;
            case operatorType::minus:
                result.numVal = left->Evaluate().numVal - right->Evaluate().numVal;
                return result;
            case operatorType::devide:
                result.numVal = left->Evaluate().numVal / right->Evaluate().numVal;
                return result;
                
            case operatorType::multiply:
                result.numVal = left->Evaluate().numVal * right->Evaluate().numVal;
                return result;
                
        
        };
    }
    
    enum class operatorType{ plus,minus,multiply,devide} mOperator;
    
};


struct UnaryExpression :Expression
{
    std::unique_ptr<Expression> sub;
    enum class operatorType{ minus,grouping} mOperator;
    Value Evaluate()
    {
        Value result;

        switch(mOperator)
        {
            case operatorType::minus:
                result.numVal = - sub->Evaluate().numVal ;
                return result;
                
            case operatorType::grouping:
                result = sub->Evaluate();
                return result;
            
        }
    
    }
    
};


struct TerminalExpression :Expression
{
    Value sub;
    Value Evaluate()
    {
        return sub;
    }
};


struct GetExpression :Expression
{
    std::string varName;
    Value Evaluate();
    
};












struct LetStatement:public  Statement{
    
    
public:
    
    std::string variablename;
    
    std::unique_ptr<Expression> rvalue;

    
    
    
    
};




#endif /* AST_hpp */
