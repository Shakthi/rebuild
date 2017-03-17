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
#include "nlohmann/json.hpp" //TODO move all to archive format
#include "cereal/cereal.hpp"
#include "cereal/types/vector.hpp"

struct Statement {
    std::string sourceText; //Debug only
    virtual ~Statement(){}
    
    
    virtual nlohmann::json ToJson();
    virtual void FromJson(nlohmann::json );
    
    
    
    template<class Archive>
    void serialize( Archive & ar )
    { ar( CEREAL_NVP(sourceText) ); }
    
};



struct UnProcessedStatment:public Statement {
    
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


struct IfStatment:public Statement {
    
    class RelationalExpression * expression;
};

struct ForStatment:public Statement {
    
        std::string forVar;
        std::unique_ptr<Expression> forBegin,forEnd,forStep;
        bool foundNext;
 
};




struct ReadStatement:public  Statement{
public:
    std::string prompt;
    std::list<std::string> variableList;
    
    
};




struct PrintStatement:public  Statement{
    
    
public:
    
    std::vector<std::unique_ptr<Expression>> printitems;
  
    
    template<class Archive>
    void serialize( Archive & ar )
    { ar( CEREAL_NVP(printitems) ); }
    
}
;




struct Expression
{
    Value::Evaluetype valuetype;

    virtual Value Evaluate()=0;
    virtual ~Expression(){}
    
    
    
    template<class Archive>
    void serialize( Archive & ar )
    { ar( CEREAL_NVP(valuetype) ); }

};


struct ExpressionList:Expression
{
    std::vector<std::unique_ptr<Expression>> list;
    Value Evaluate(){return Value();}
    
};




struct BainaryExpression :Expression
{
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
    
    
    template<class Archive>
    void serialize( Archive & ar )
    { ar( CEREAL_NVP(left),CEREAL_NVP(right) ); }

};

struct ArithmeticExpression:BainaryExpression {
    
    enum class operatorType{ plus,minus,multiply,devide} mOperator;
    Value Evaluate();

    
    
    template<class Archive>
    void serialize( Archive & ar )
    { ar( CEREAL_NVP(mOperator)); }
    
};


struct RelationalExpression :BainaryExpression
{
    enum class operatorType{ less,equal,greater} mOperator;
    
    Value::Evaluetype inputype;

    RelationalExpression(Expression * aleft,operatorType ot,Expression * aright,Value::Evaluetype ainputtype)
    {
        left=std::unique_ptr<Expression>(aleft);
        right=std::unique_ptr<Expression>(aright);
        mOperator = ot;
        valuetype = Value::Evaluetype::booltype;
        inputype = ainputtype;
    }
    Value Evaluate();
    
    template<class Archive>
    void serialize( Archive & ar )
    { ar( CEREAL_NVP(mOperator),CEREAL_NVP(inputype)); }
};



struct UnaryExpression :Expression
{
    std::unique_ptr<Expression> sub;
    enum class operatorType{ minus,grouping} mOperator;
    Value Evaluate()
    {

        switch(mOperator)
        {
            case operatorType::minus:
                return Value( - sub->Evaluate().getNumVal() ) ;
                
                
            case operatorType::grouping:
                return Value(  sub->Evaluate().getNumVal() ) ;
            
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
    
    
    
    template<class Archive>
    void serialize( Archive & ar )
    { ar( CEREAL_NVP(varName) ); }
    
};












struct LetStatement:public  Statement{
    
    
public:
    
    std::string variablename;
    
    std::unique_ptr<Expression> rvalue;

    
    
    
    
};




#endif /* AST_hpp */
