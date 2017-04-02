//
//  AST.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 13/02/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "AST.hpp"
#include "quickbasic.h"
#include "cereal/types/memory.hpp"

#include "Logger.hpp"




#include <assert.h>
#include <sstream>


Value GetExpression::Evaluate(){
    
    Value v=varTable[varName];
    assert(v.valutype == valuetype || Value::Evaluetype::emptyType == v.valutype);
    
    return v;
    

}



Value ArithmeticExpression::Evaluate()
{
    
    switch(valuetype)
    {
        case  Value::Evaluetype::stringtype:
            switch(mOperator)
            {
                case operatorType::plus:
                    return Value ( left->Evaluate().getStringVal() + right->Evaluate().getStringVal());
                    
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
                        return Value ( left->Evaluate().getNumVal() - right->Evaluate().getNumVal());;
                        
                    case operatorType::plus:
                        return Value ( left->Evaluate().getNumVal() + right->Evaluate().getNumVal());
                        
                        
                    case operatorType::devide:
                        return Value (left->Evaluate().getNumVal() / right->Evaluate().getNumVal());
                        
                        
                    case operatorType::multiply:
                        return Value( left->Evaluate().getNumVal() * right->Evaluate().getNumVal());
                };
            
        default:
            assert(false);
            return Value();
    }
    
    
    
    }








Value RelationalExpression::Evaluate()
{
    
    switch(inputype)
    {
        case  Value::Evaluetype::stringtype:
            switch(mOperator)
        {
            case operatorType::equal:
                return Value ( left->Evaluate().getStringVal() == right->Evaluate().getStringVal());
                
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
                return Value ( left->Evaluate().getNumVal()  == right->Evaluate().getNumVal());;
                
            case operatorType::less:
                return Value ( left->Evaluate().getNumVal() < right->Evaluate().getNumVal());
                
                
            case operatorType::greater:
                return Value (left->Evaluate().getNumVal() > right->Evaluate().getNumVal());
            
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
CEREAL_REGISTER_TYPE (ForStatment);
CEREAL_REGISTER_TYPE (ReadStatement);
CEREAL_REGISTER_TYPE(PrintStatement);
CEREAL_REGISTER_TYPE(LetStatement);


CEREAL_REGISTER_POLYMORPHIC_RELATION(Statement, LetStatement);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Statement, PrintStatement);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Statement,UnProcessedStatment);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Statement,EndStatement);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Statement,NextStatement);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Statement,RemarkStatement);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Statement,IfStatment);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Statement,ForStatment);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Statement,ReadStatement);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Statement,ErrorStatement);









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



nlohmann::json Statement::ToJson()
{
//    Rlog rlog;
    
    nlohmann::json root;
    root["class"]="Statement";
    
    std::ostringstream os;
    cereal::JSONOutputArchive oarchive( os );

    
     oarchive(make_smart(this));
   
    std::string datastring = os.str()+"}"; //Bug? Really
    
    rlog<<datastring<<std::endl;

    nlohmann::json value0=nlohmann::json::parse(datastring);
    
    root["content"]=value0["value0"];
    
    
    return root;

}


Statement * Statement::GetFromJson(nlohmann::json root)
{
    assert(root["class"]=="Statement");
    //Rlog rlog;

    nlohmann::json value0;
    value0["value0"]=root["content"];
    
    std::stringstream is;
    is<<value0;
    rlog<<is.str()<<std::endl;
    
   cereal::JSONInputArchive iarchive( is );
    
    
    auto deleter=[&](Statement* ){};
    std::unique_ptr<Statement, decltype(deleter)> st(nullptr, deleter);
    
    
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

template<class Archive>
void ForStatment::serialize( Archive & ar )
{
    ar( CEREAL_NVP(forVar),CEREAL_NVP(forBegin),CEREAL_NVP(forEnd),CEREAL_NVP(forStep) );
    ar( CEREAL_NVP(sourceText));
    

    std::vector<std::unique_ptr<Statement>> statementsptr;
    statementsptr.reserve(statements.size());
    
    
    for(auto st : statements )
    {
        statementsptr.push_back( std::unique_ptr<Statement>(st));
    
    }
    
    ar( CEREAL_NVP(statementsptr));
    
    if(statements.empty() && !statementsptr.empty())
    {
        statements.reserve(statementsptr.size());
        for(auto  & st : statementsptr )
        {
            statements.push_back(st.release());
            
        }
    
    }
    
    
        
}


 std::string ForStatment::dumpToString()const{
    return std::string("for ") + forVar + " = "
    + forBegin->dumpToString()
    + " to "
    + forEnd->dumpToString() + ((statements.empty())?"":":" );
}

