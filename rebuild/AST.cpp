//
//  AST.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 13/02/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "AST.hpp"
#include "quickbasic.h"
#include <assert.h>


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




