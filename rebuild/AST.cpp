//
//  AST.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 13/02/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "AST.hpp"
#include "Value.h"
#include "cereal/types/memory.hpp"

#include "Logger.hpp"




#include <assert.h>
#include <sstream>


Value GetExpression::Evaluate(VarTable * varTable){
    
    Value v=varTable->GetValue(varName);
    assert(v.valutype == valuetype || Value::Evaluetype::emptyType == v.valutype);
    
    return v;
    

}



Value ArithmeticExpression::Evaluate(VarTable * varTable)
{
    
    switch(valuetype)
    {
        case  Value::Evaluetype::stringtype:
            switch(mOperator)
            {
                case operatorType::plus:
                    return Value ( left->Evaluate(varTable).getStringVal() + right->Evaluate(varTable).getStringVal());
                    
                default:
                    assert(false);//should not come here
                    return Value();
                    
            }            

            
        case  Value::Evaluetype::floattype:
            assert(valuetype == Value::Evaluetype::floattype &&
                   left->valuetype == Value::Evaluetype::floattype &&
                   right->valuetype == Value::Evaluetype::floattype
                   );
            

            
                switch(mOperator)
                {
                    case operatorType::minus:
                        return Value ( left->Evaluate(varTable).getNumVal() - right->Evaluate(varTable).getNumVal());;
                        
                    case operatorType::plus:
                        return Value ( left->Evaluate(varTable).getNumVal() + right->Evaluate(varTable).getNumVal());
                        
                        
                    case operatorType::devide:
                        return Value (left->Evaluate(varTable).getNumVal() / right->Evaluate(varTable).getNumVal());
                        
                        
                    case operatorType::multiply:
                        return Value( left->Evaluate(varTable).getNumVal() * right->Evaluate(varTable).getNumVal());
                };
            
        default:
            assert(false);
            return Value();
    }
    
    
    
    }








Value RelationalExpression::Evaluate(VarTable * varTable)
{
    
    switch(inputype)
    {
        case  Value::Evaluetype::stringtype:
            switch(mOperator)
        {
            case operatorType::equal:
                return Value ( left->Evaluate(varTable).getStringVal() == right->Evaluate(varTable).getStringVal());
                
            default:
                assert(false);//should not come here
                return Value();
                
        }
            
            
        case  Value::Evaluetype::floattype:
            assert(valuetype == Value::Evaluetype::booltype &&
                   left->valuetype == Value::Evaluetype::floattype &&
                   right->valuetype == Value::Evaluetype::floattype
                   );
            
            
            
            switch(mOperator)
        {
            case operatorType::equal:
                return Value ( left->Evaluate(varTable).getNumVal()  == right->Evaluate(varTable).getNumVal());;
                
            case operatorType::less:
                return Value ( left->Evaluate(varTable).getNumVal() < right->Evaluate(varTable).getNumVal());
                
                
            case operatorType::greater:
                return Value (left->Evaluate(varTable).getNumVal() > right->Evaluate(varTable).getNumVal());
            
        };
            
            
        default:
            assert(false);
            return Value();
    }
    
    
    
}




CEREAL_REGISTER_TYPE (UnProcessedStatment);
CEREAL_REGISTER_TYPE (ErrorStatement);
CEREAL_REGISTER_TYPE (EndStatement);
CEREAL_REGISTER_TYPE (NextStatement);
CEREAL_REGISTER_TYPE (RemarkStatement);
CEREAL_REGISTER_TYPE (IfStatment);
CEREAL_REGISTER_TYPE (ForStatement);
CEREAL_REGISTER_TYPE (ReadStatement);
CEREAL_REGISTER_TYPE(PrintStatement);
CEREAL_REGISTER_TYPE(PrintElementStatement);
CEREAL_REGISTER_TYPE(LetStatement);

CEREAL_REGISTER_TYPE(ListCommand);
CEREAL_REGISTER_TYPE(CustomCommand);



CEREAL_REGISTER_POLYMORPHIC_RELATION(Statement, LetStatement);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Statement, PrintStatement);
CEREAL_REGISTER_POLYMORPHIC_RELATION(PrintStatement,PrintElementStatement);


CEREAL_REGISTER_POLYMORPHIC_RELATION(Statement,UnProcessedStatment);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Statement,EndStatement);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Statement,NextStatement);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Statement,RemarkStatement);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Statement,IfStatment);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Statement,ForStatement);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Statement,ReadStatement);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Statement,ErrorStatement);


CEREAL_REGISTER_POLYMORPHIC_RELATION(Command,ListCommand);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Command,CustomCommand);

CEREAL_REGISTER_POLYMORPHIC_RELATION(Sentence,Command);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Sentence, Statement);








CEREAL_REGISTER_TYPE(TerminalExpression);
CEREAL_REGISTER_TYPE(GetExpression);
CEREAL_REGISTER_TYPE(UnaryExpression);
CEREAL_REGISTER_TYPE(BainaryExpression);
CEREAL_REGISTER_TYPE(ArithmeticExpression);
CEREAL_REGISTER_TYPE(RelationalExpression);



CEREAL_REGISTER_POLYMORPHIC_RELATION(Expression, GetExpression);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Expression, UnaryExpression);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Expression, TerminalExpression);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Expression, BainaryExpression);
CEREAL_REGISTER_POLYMORPHIC_RELATION(BainaryExpression,RelationalExpression );
CEREAL_REGISTER_POLYMORPHIC_RELATION(BainaryExpression,ArithmeticExpression );




#include "cereal/archives/json.hpp"

template <class T>
void null_deleter(T *)
{

}
template <class T>
auto make_smart(T* t)-> std::unique_ptr <T,void (*)(T*)>
{
    return std::unique_ptr<T,void (*)(T*)>(t,null_deleter);
}



nlohmann::json Sentence::ToJson()
{
//    Rlog rlog;
    
    nlohmann::json root;
    root["class"]="Sentence";
    
    std::ostringstream os;
    cereal::JSONOutputArchive oarchive( os );

    
     oarchive(make_smart(this));
   
    std::string datastring = os.str()+"}"; //Bug? Really
    
    rlog<<datastring<<std::endl;

    nlohmann::json value0=nlohmann::json::parse(datastring);
    
    root["content"]=value0["value0"];
    
    
    return root;

}


Sentence * Sentence::GetFromJson(nlohmann::json root)
{
    assert(root["class"]=="Sentence");
    //Rlog rlog;

    nlohmann::json value0;
    value0["value0"]=root["content"];
    
    std::stringstream is;
    is<<value0;
    rlog<<is.str()<<std::endl;
    
   cereal::JSONInputArchive iarchive( is );
    
    
    auto deleter=[&](Sentence* ){};
    std::unique_ptr<Sentence, decltype(deleter)> st(nullptr, deleter);
    
    
    iarchive(st);
    
    return st.get();
  
}




std::string PrintStatement::dumpToString()const
{
    std::string result="print ";
    
    
    bool isFirst=true;
    for (auto const &  i: printitems)
    {
        if(!isFirst)
            result+="; ";
            
            
        result+=i->dumpToString();
        
        isFirst=false;
    }
    return result;
}


std::string PrintElementStatement::dumpToString()const
{
    std::string result="printe ";
    
    
    bool isFirst=true;
    for (auto const &  i: printitems)
    {
        if(!isFirst)
            result+="; ";
        
        
        result+=i->dumpToString();
        
        isFirst=false;
    }
    return result;
}



std::string ReadStatement::dumpToString()const
{
    std::string result="input ";
    if(prompt!="")
        result+="\""+prompt+"\"; ";
        
    
    
    bool isFirst=true;
    for (auto const &  i: variableList)
    {
        if(!isFirst)
            result+=";";
        
        
        result+=i;
        
        isFirst=false;
    }
    return result;
}


std::string ArithmeticExpression::dumpToString()const
{
    switch(mOperator)
    {
        case operatorType::minus:
            return left->dumpToString() +"-"+ right->dumpToString();;
            
        case operatorType::plus:
            return left->dumpToString() +"+"+ right->dumpToString();;
            
            
        case operatorType::devide:
            return left->dumpToString() +"/"+ right->dumpToString();;
            
            
        case operatorType::multiply:
            return left->dumpToString() +"*"+ right->dumpToString();;
    };


}


std::string UnaryExpression::dumpToString()const
{
    
    switch(mOperator)
    {
        case operatorType::minus:
            return "-"+sub->dumpToString();
            
            
        case operatorType::grouping:
            return "("+sub->dumpToString()+")";
            
    }




}



std::string TerminalExpression::dumpToString()const
{
    
    switch(sub.valutype)
    {
            
        default: return "<NULL>";
            break;
            
        case Value::Evaluetype::stringtype:
        {
            return std::string("\"")+sub.getStringVal()+std::string("\"");
        }
            break;
        case Value::Evaluetype::booltype:
        {
            return sub.getBoolVal()?"TRUE":"FALSE";
            
        }
            break;
            
        case Value::Evaluetype::floattype:
        {
            
            std::stringstream s;
            s<<sub.getNumVal();
            
            return s.str();
            
        }
            break;
            
            
            
    }
    
   
}

std::string RelationalExpression::dumpToString()const
{
    
    
    switch(mOperator)
    {
        case operatorType::equal:
            return left->dumpToString() + "=" + right->dumpToString();
            
        case operatorType::less:
            return left->dumpToString() + "<" + right->dumpToString();
            
            
        case operatorType::greater:
            return left->dumpToString() + ">" + right->dumpToString();
            
    };

    
}

void Deleter(Statement* )
{}

struct ii{}
;

template<class Archive>
void ForStatement::serialize( Archive & ar )
{
    ar( CEREAL_NVP(var),CEREAL_NVP(forBegin),CEREAL_NVP(forEnd),CEREAL_NVP(forStep) );
    ar( CEREAL_NVP(sourceText));

     ar( CEREAL_NVP(statements));
    

        
}


 std::string ForStatement::dumpToString()const{
    return std::string("for ") + var + " = "
    + forBegin->dumpToString()
    + " to "
    + forEnd->dumpToString() + ((statements.empty())?"":":" );
}


ExpressionList::ExpressionList(const ExpressionList & other)
:Expression(other)

{

    for(const auto &st : other.list )
    {
        list.push_back( std::unique_ptr<Expression>(st->clone()));

    }


}


UnaryExpression::UnaryExpression(const UnaryExpression & other)
:Expression(other)
{

    sub =std::unique_ptr<Expression>(other.sub->clone());
    mOperator =other.mOperator;

    

}

BainaryExpression::BainaryExpression(const BainaryExpression & other)
:Expression(other)

{

    left =std::unique_ptr<Expression>(other.left->clone());
    right =std::unique_ptr<Expression>(other.left->clone());



    
}


LetStatement::LetStatement(const LetStatement & other)
:Statement(other)

{
    rvalue =std::unique_ptr<Expression>(other.rvalue->clone());

}

IfStatment::IfStatment(const IfStatment & other)
:Statement(other)

{
    expression =std::unique_ptr<RelationalExpression>(other.expression->clone());

}

PrintStatement::PrintStatement(const PrintStatement & other)
:Statement(other)

{
    for(const auto &st : other.printitems )
    {
        printitems.push_back( std::unique_ptr<Expression>(st->clone()));

    }
    

}



ForStatement::ForStatement(const ForStatement & other)
:Statement(other)
{

    for(const auto &st : other.statements )
    {
        statements.push_back( StatementRef(st->clone()));

    }

    var=other.var;
    forBegin = std::unique_ptr<Expression>(other.forBegin->clone());
    forEnd = std::unique_ptr<Expression>(other.forEnd->clone());
    forStep = std::unique_ptr<Expression>(other.forStep->clone());




}



