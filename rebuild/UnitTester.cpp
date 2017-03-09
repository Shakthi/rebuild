//
//  UnitTester.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 10/03/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "UnitTester.hpp"
#include "Logger.cpp"
#include "BasicStepProccessor.cpp"



UnitTester::~UnitTester()
{

    for(auto acase:testCases)
    {
        delete acase;
    }

}

using namespace nlohmann;

nlohmann::json
UnitTester::TestCase::ToJson()
{
    json root;
    root["input"] = input;
    root["output"] = output;
    
    return root;
}


void UnitTester::TestCase::FromJson(nlohmann::json root)
{
    input = root["input"];
    output = root["output"];
    


}



nlohmann::json
UnitTester::ToJson()
{
    using namespace nlohmann;
    
    json root;
    json cases;
    
    for(auto acase:testCases)
    {
        cases.push_back(acase->ToJson());
    }
    
    
    
    root["content"] = cases;
    root["type"] = "testcases";
    
    
    return root;
}
void UnitTester::FromJson(nlohmann::json root)
{
    using namespace nlohmann;
    
    json cases = root["cases"];
    for (int i = 0; i < cases.size(); i++) {
        
    }
}


void UnitTester::TestAll(Bas)
{
    int total=0;
    int passed=0;
    
    for(auto acase:testCases)
    {
        if(acase->Test())
            passed++;
        total++;
    }
    
    rlog<<"Test ended:total"<<total<<"Passed"<<std::endl;

}




void UnitTester::TestCase::FromJson(nlohmann::json root)
{
    input = root["input"];
    output = root["output"];
    
    
    
}
