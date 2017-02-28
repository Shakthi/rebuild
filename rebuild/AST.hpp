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
#include "quickbasic.h"


struct Statement {
    
    std::string sourceText;
    virtual ~Statement(){}
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



struct ForStatment:public Statement {
    
    struct ForBlock
    {
        std::string forVar;
        float  forBegin;
        float  forEnd;
        
        float  forStep;
        
        bool foundNext;
        
        
    } forBlock;

};




struct ReadStatement:public  Statement{
public:
    std::string prompt;
    std::list<std::string> variableList;
    
    
};




struct PrintStatement:public  Statement{
    
    
public:
    
    std::string content;
    
    
    
};






struct LetStatement:public  Statement{
    
    
public:
    
    std::string variablename;
    Value val;
    
    
    
    
};




#endif /* AST_hpp */
