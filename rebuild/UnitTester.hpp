//
//  UnitTester.hpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 10/03/17.
//  Copyright © 2017 self. All rights reserved.
//

#ifndef UnitTester_hpp
#define UnitTester_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include "Rebuild.hpp"
#include "Logger.hpp"



class UnitTester:public Rebuild::Serialised {
    
    
    Rlog rlog;

    
public:
    
    struct TestCase
    {
          std::string  input;
          std::string  output;
        virtual bool Test();
        nlohmann::json ToJson();
        void FromJson(nlohmann::json);
    };
    
    
    
    void TestAll();
    
    std::vector<TestCase*> testCases;
    
    
    ~UnitTester();
    
    nlohmann::json ToJson();
    void FromJson(nlohmann::json);

    

};

#endif /* UnitTester_hpp */
