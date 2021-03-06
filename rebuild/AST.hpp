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
#include "Parser/quickbasic.h"
#include "nlohmann/json.hpp"
#include "cereal/cereal.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/list.hpp"


struct Statement {
    std::string sourceText; //Debug only
    virtual ~Statement(){}
    
    
    nlohmann::json ToJson();
    static  Statement * GetFromJson(nlohmann::json );
    
    
    
    template<class Archive>
    void serialize( Archive & ar )
    { ar( CEREAL_NVP(sourceText) ); }
    
    
    virtual bool isEqual(const Statement & that){ return this -> dumpToString()  == that.dumpToString();}
    
    virtual std::string dumpToString()const {return sourceText;}
    
};



struct UnProcessedStatment:public Statement {
  
    template<class Archive>
    void serialize( Archive & ar )
    { ar( CEREAL_NVP(sourceText)); }

};


struct ErrorStatement:public Statement {
    std::string description;
    
    ErrorStatement(std::string aDescription):description(aDescription){}
    template<class Archive>
    void serialize( Archive & ar )
    { ar( CEREAL_NVP(description),CEREAL_NVP(sourceText)); }

    
    ErrorStatement(){}

    
};



struct EndStatement:public  Statement{
    template<class Archive>
    void serialize( Archive & ar )
    { ar( CEREAL_NVP(sourceText)); }

};


struct NextStatement:public  Statement{
    template<class Archive>
    void serialize( Archive & ar )
    { ar( CEREAL_NVP(sourceText)); }

};

struct ListStatement:public  Statement{
    
};



struct RemarkStatement:public  Statement{
    std::string comments;

    
    template<class Archive>
    void serialize( Archive & ar )
    { ar( CEREAL_NVP(comments)); ar( CEREAL_NVP(sourceText));  }
};



struct ForStatment:public Statement {
    
        std::string forVar;
        std::unique_ptr<Expression> forBegin,forEnd,forStep;
    
        std::vector<Statement*>  statements;
    
    
    
    
        bool foundNext;
        template<class Archive>
        void serialize( Archive & ar );
    
     std::string dumpToString()const;

 
};




struct ReadStatement:public  Statement{
public:
    std::string prompt;
    std::list<std::string> variableList;
    
    
    
    
    template<class Archive>
    void serialize( Archive & ar )
    { ar( CEREAL_NVP(prompt),CEREAL_NVP(variableList),CEREAL_NVP(sourceText) );
    }
    
    std::string dumpToString()const;

    
};




struct PrintStatement:public  Statement{
    
    
public:
    
    std::vector<std::unique_ptr<Expression>> printitems;
  
    
    template<class Archive>
    void serialize( Archive & ar )
    { ar( CEREAL_NVP(printitems),CEREAL_NVP(sourceText) ); }
    
    
    std::string dumpToString()const;

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
    
    
    virtual std::string dumpToString()const{return "";}


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
    { ar( CEREAL_NVP(left),CEREAL_NVP(right),CEREAL_NVP_("::base",cereal::base_class<Expression>( this ) )); }
    
    
    

};

struct ArithmeticExpression:BainaryExpression {
    
    enum class operatorType{ plus,minus,multiply,devide} mOperator;
    Value Evaluate();

    
    
    template<class Archive>
    void serialize( Archive & ar )
    {
        ar( CEREAL_NVP(mOperator),CEREAL_NVP_("::base",cereal::base_class<BainaryExpression>( this )));
    
    }
    
    
     std::string dumpToString()const;
    
    
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
    {

        ar( CEREAL_NVP(mOperator),CEREAL_NVP(inputype),CEREAL_NVP_("::base",cereal::base_class<BainaryExpression>( this )));
        

    }
    
    RelationalExpression(){}
    
     std::string dumpToString()const;
};




struct IfStatment:public Statement {
    
    std::unique_ptr<RelationalExpression>   expression;
    
    template<class Archive>
    void serialize( Archive & ar )
    { ar( CEREAL_NVP(expression) ); }
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
    
    
    template<class Archive>
    void serialize( Archive & ar )
    { ar( CEREAL_NVP(mOperator),CEREAL_NVP(sub) ,CEREAL_NVP_("::base",cereal::base_class<Expression>( this ))); }

    
    std::string dumpToString()const;

    
};


struct TerminalExpression :Expression
{
    Value sub;
    Value Evaluate()
    {
        return sub;
    }
    
    template<class Archive>
    void serialize( Archive & ar )
    {
           ar( CEREAL_NVP_("::base",cereal::base_class<Expression>( this )));
        
        ar( CEREAL_NVP(sub.valutype) );
        
        valuetype = sub.valutype;
    
        switch(sub.valutype)
        {
                
            default: 
                break;
                
            case Value::Evaluetype::stringtype:
            {
                std::string stringValue = sub.getStringVal() ;
                ar(CEREAL_NVP(stringValue));
                sub = Value(stringValue);
            
            }
                break;
            case Value::Evaluetype::booltype:
            {
                bool boolValue = sub.getBoolVal();
                ar(CEREAL_NVP(boolValue));
                sub = Value(boolValue);
                
            }
                break;
                
            case Value::Evaluetype::floattype:
            {
                float numVal = sub.getNumVal();
                ar(CEREAL_NVP(numVal));
                sub = Value(numVal);
                
            }
                break;
                
        
                
        }
    
        
    
    }
    
    
     std::string dumpToString()const;

    
    
    
    
};





struct GetExpression :Expression
{
    std::string varName;
    Value Evaluate();
    
    
    
    template<class Archive>
    void serialize( Archive & ar )
    {
            ar( CEREAL_NVP_("::base",cereal::base_class<Expression>( this )));
        
        ar( CEREAL_NVP(varName) ); }
    
    std::string dumpToString()const{return varName;}

    
};












struct LetStatement:public  Statement{
    
    
public:
    
    std::string variablename;
    
    std::unique_ptr<Expression> rvalue;

    
    
    template<class Archive>
    void serialize( Archive & ar )
    { ar( CEREAL_NVP(variablename),CEREAL_NVP(rvalue)),CEREAL_NVP(sourceText);; }
    
    std::string dumpToString()const{return "let "+variablename+" = "+ rvalue->dumpToString();}

    
};




#endif /* AST_hpp */
