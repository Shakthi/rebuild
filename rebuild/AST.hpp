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
#include <vector>
#include <string>


struct Statement {
    
    
public:
    virtual std::string Describe(){return "";}
};


struct ErrorStatement:public Statement {
    std::string description;
    ErrorStatement(std::string aDescription):description(aDescription){}
public:
    
};



struct EndStatement:public  Statement{
};

struct ReadStatement:public  Statement{
    
    
public:
    
    std::string prompt;
    std::vector<std::string> variableList;
    
    
};

#endif /* AST_hpp */
