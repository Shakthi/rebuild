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
         // std::string  input;
          //std::string  output;
        //virtual bool Test();
        
        enum class DataType{
            input,
            output
        };
        
        struct Data
        {
            DataType dataType;
            std::string data;
        };
        
        
        
        
        std::vector<Data>  testdata;
        
        nlohmann::json ToJson();
        void FromJson(nlohmann::json);
        
        
        void AddInput(std::string );
        void AddOutput(std::string );
        
        
        bool Test(){return true;}
    };
    
    
    
    void TestAll();
    
    std::vector<TestCase*> testCases;
    
    
    ~UnitTester();
    
    nlohmann::json ToJson();
    void FromJson(nlohmann::json);
    
    
    void AddInput(std::string );
    void AddOutput(std::string );
    


private:
    TestCase currentSession;

    

};

#endif /* UnitTester_hpp */
