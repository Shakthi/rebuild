//
//  ParserWrapper.hpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 13/02/17.
//  Copyright © 2017 self. All rights reserved.
//

#ifndef ParserWrapper_hpp
#define ParserWrapper_hpp

#include <string>
#include "AST.hpp"



class Parser {
    
    
public:
    Statement  Parse(std::string data);
};


class BasicParser :public Parser{
    
    
public:
//    Statement  Parse(std::string data);
};







#endif /* ParserWrapper_hpp */
