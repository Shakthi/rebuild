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
  assert(v.valutype == valuetype);
    
    return v;
    

}



Value BainaryExpression::Evaluate()
{
    Value result;
    
    switch(valuetype)
    {
        case  Value::Evaluetype::stringtype:
            switch(mOperator)
            {
                case operatorType::plus:
                    result.stringVal = left->Evaluate().stringVal + right->Evaluate().stringVal;
                    result.valutype = Value::Evaluetype::stringtype;
                    return result;
                default:
                    assert(false);//should not come here
                    return result;
                    
            }
            
        case  Value::Evaluetype::floattype:
            assert(valuetype == Value::Evaluetype::floattype &&
                   left->valuetype == Value::Evaluetype::floattype &&
                   right->valuetype == Value::Evaluetype::floattype
                   );
            

            
                switch(mOperator)
                {
                    case operatorType::plus:
                        result.numVal = left->Evaluate().numVal + right->Evaluate().numVal;
                        result.valutype = Value::Evaluetype::floattype;

                        return result;
                    case operatorType::minus:
                        result.numVal = left->Evaluate().numVal - right->Evaluate().numVal;
                        result.valutype = Value::Evaluetype::floattype;
                        
                        return result;
                    case operatorType::devide:
                        result.numVal = left->Evaluate().numVal / right->Evaluate().numVal;
                        result.valutype = Value::Evaluetype::floattype;
                        
                        return result;
                        
                    case operatorType::multiply:
                        result.numVal = left->Evaluate().numVal * right->Evaluate().numVal;
                        result.valutype = Value::Evaluetype::floattype;
                        return result;
                };
            
        default:
            assert(false);
            return result;
    }
    
    
    
    
    result.valutype = Value::Evaluetype::floattype;
    
    }






