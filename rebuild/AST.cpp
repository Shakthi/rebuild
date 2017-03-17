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



CEREAL_REGISTER_TYPE(PrintStatement);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Statement, PrintStatement);



CEREAL_REGISTER_TYPE(GetExpression);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Expression, GetExpression);


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
    
    

    //Rlog rlog;

    
    nlohmann::json root;
    root["class"]="Statement";
    
    std::ostringstream os;
    cereal::JSONOutputArchive oarchive( os );

    auto noop_deleter = [](Statement*) {};
    //oarchive(std::unique_ptr<Statement,decltype(noop_deleter)>(this,noop_deleter));
    
     oarchive(make_smart(this));
   
    std::string datastring = os.str()+"}"; //Bug? Really
    
    rlog<<datastring<<std::endl;

    nlohmann::json value0=nlohmann::json::parse(datastring);
    
    root["content"]=value0["value0"];
    
    
    return root;

}
void Statement::FromJson(nlohmann::json root)
{
    assert(root["class"]=="Statement");
    
}












