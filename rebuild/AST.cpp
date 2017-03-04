//
//  AST.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 13/02/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "AST.hpp"
#include "quickbasic.h"


Value GetExpression::Evaluate(){
    return varTable[varName];

}





