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
#include "VarTable.hpp"
#include "Value.h"
#include "cereal/types/vector.hpp"
#include "cereal/types/list.hpp"

#include "Sentence.h"





struct UnProcessedStatment:public Statement {
  
    template<class Archive>
    void serialize( Archive & ar )
    { ar( CEREAL_NVP(sourceText)); }

    UnProcessedStatment  * clone ()const { return new UnProcessedStatment(*this); }


};


struct ErrorStatement:public Statement {
    std::string description;
    
    ErrorStatement(std::string aDescription):description(aDescription){}
    template<class Archive>
    void serialize( Archive & ar )
    { ar( CEREAL_NVP(description),CEREAL_NVP(sourceText)); }

    
    ErrorStatement(){}
    ErrorStatement  * clone ()const { return new ErrorStatement(*this); }




    
};



struct EndStatement:public  Statement{
    template<class Archive>
    void serialize( Archive & ar )
    { ar( CEREAL_NVP(sourceText)); }
    EndStatement  * clone ()const { return new EndStatement(*this); }


};


struct NextStatement:public  Statement{
    template<class Archive>
    void serialize( Archive & ar )
    { ar( CEREAL_NVP(sourceText)); }
    NextStatement  * clone ()const { return new NextStatement(*this); }


};

struct ListCommand:public  Command{
    ListCommand  * clone ()const { return new ListCommand(*this); }


};


struct CustomCommand:public  Command{
    std::string name;
    CustomCommand  * clone ()const { return new CustomCommand(*this); }

};



struct RemarkStatement:public  Statement{
    std::string comments;

    
    template<class Archive>
    void serialize( Archive & ar )
    { ar( CEREAL_NVP(comments)); ar( CEREAL_NVP(sourceText));  }

    RemarkStatement  * clone ()const { return new RemarkStatement(*this); }

};



struct ForStatment:public Statement {
    
        std::string forVar;
        std::unique_ptr<Expression> forBegin,forEnd,forStep;
    
        std::vector<StatementRef>  statements;
        
    
    
    
    
        bool foundNext;
        template<class Archive>
        void serialize( Archive & ar );
    
        std::string dumpToString()const;

        ForStatment(){}

        ForStatment(const ForStatment & other);
        ForStatment  * clone ()const { return new ForStatment(*this); }

        ~ForStatment()
        {


        }
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
    ReadStatement  * clone ()const { return new ReadStatement(*this); }


};




struct PrintStatement:public  Statement{
    
    
public:
    
    std::vector<std::unique_ptr<Expression>> printitems;
  
    
    template<class Archive>
    void serialize( Archive & ar )
    { ar( CEREAL_NVP(printitems),CEREAL_NVP(sourceText) ); }
    
    
    std::string dumpToString()const;


    PrintStatement(const PrintStatement & other);
    PrintStatement(){}
    PrintStatement  * clone ()const { return new PrintStatement(*this); }


}
;


struct PrintElementStatement:public  PrintStatement{
    
    
public:
    
    template<class Archive>
    void serialize( Archive & ar )
    { ar( CEREAL_NVP(printitems),CEREAL_NVP(sourceText) ); }
    
    
    
    std::string dumpToString()const;
    PrintElementStatement  * clone ()const { return new PrintElementStatement(*this); }


}
;






struct Expression
{
    Value::Evaluetype valuetype;

    virtual Value Evaluate(VarTable * varTable)=0;
    virtual Expression *clone() const=0;



    virtual ~Expression(){}
    
    
    
    template<class Archive>
    void serialize( Archive & ar )
    { ar( CEREAL_NVP(valuetype) ); }
    
    
    virtual std::string dumpToString()const{return "";}


};


struct ExpressionList:Expression
{
    std::vector<std::unique_ptr<Expression>> list;
    Value Evaluate( VarTable * varTable){return Value();}
    ExpressionList * clone ()const { return new ExpressionList(*this); }

    ExpressionList(const ExpressionList & other);

    ExpressionList(){}
};




struct BainaryExpression :Expression
{
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
    
    
    template<class Archive>
    void serialize( Archive & ar )
    { ar( CEREAL_NVP(left),CEREAL_NVP(right),CEREAL_NVP_("::base",cereal::base_class<Expression>( this ) )); }
    
    
    BainaryExpression(const BainaryExpression & other);
    BainaryExpression(){};


};

struct ArithmeticExpression:BainaryExpression {
    
    enum class operatorType{ plus,minus,multiply,devide} mOperator;
    Value Evaluate( VarTable * varTable);
    ArithmeticExpression * clone ()const { return new ArithmeticExpression(*this); }

    
    
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
    Value Evaluate( VarTable * varTable);
    RelationalExpression * clone ()const { return new RelationalExpression(*this); }


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

    IfStatment(const IfStatment & other);
    IfStatment  * clone ()const { return new IfStatment(*this); }

    IfStatment(){}

};


struct UnaryExpression :Expression
{
    std::unique_ptr<Expression> sub;
    enum class operatorType{ minus,grouping} mOperator;
    Value Evaluate( VarTable * varTable)
    
    {

        switch(mOperator)
        {
            case operatorType::minus:
                return Value( - sub->Evaluate(varTable).getNumVal() ) ;
                
                
            case operatorType::grouping:
                return Value(  sub->Evaluate(varTable).getNumVal() ) ;
            
        }
    
    }
    UnaryExpression * clone ()const { return new UnaryExpression(*this); }
    
    UnaryExpression(const UnaryExpression & that);
    UnaryExpression(){}

    
    template<class Archive>
    void serialize( Archive & ar )
    { ar( CEREAL_NVP(mOperator),CEREAL_NVP(sub) ,CEREAL_NVP_("::base",cereal::base_class<Expression>( this ))); }

    
    std::string dumpToString()const;

    
};


struct TerminalExpression :Expression
{
    Value sub;
    Value Evaluate( VarTable * varTable)
    {
        return sub;
    }
    TerminalExpression * clone ()const { return new TerminalExpression(*this); }


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
    Value Evaluate( VarTable * varTable);
    GetExpression * clone ()const { return new GetExpression(*this); }


    
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
    LetStatement  * clone ()const { return new LetStatement(*this); }
    LetStatement(const LetStatement & that);
    LetStatement(){}


    
    template<class Archive>
    void serialize( Archive & ar )
    { ar( CEREAL_NVP(variablename),CEREAL_NVP(rvalue)),CEREAL_NVP(sourceText);; }
    
    std::string dumpToString()const{return "let "+variablename+" = "+ rvalue->dumpToString();}

    
};




#endif /* AST_hpp */
