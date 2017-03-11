//
//  UnitTester.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 10/03/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "UnitTester.hpp"
#include "Logger.hpp"
#include "BasicStepProccessor.hpp"



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
    json testdataJson;
    
    for(auto testdata:testdata)
    {
        json dataJson;
        if (testdata.dataType == DataType::input) {
            dataJson["dataType"]="input";
        }
        
        if (testdata.dataType == DataType::output) {
            dataJson["dataType"]="output";
        }
        
        dataJson["data"]=testdata.data;
        
        testdataJson.push_back(dataJson);
    }
    
    
    root["session"]="session";
    root["root"]=testdataJson;
    
    return root;
}


void UnitTester::TestCase::FromJson(nlohmann::json root)
{
    
    
    
    json testdataJson= root["root"];
    
    
    for(auto dataJson:testdataJson)
    {
        Data data;
        if (dataJson["dataType"]=="input") {
            data.dataType = DataType::input;
            data.data = dataJson["data"];
            
        }
        
        if (dataJson["dataType"]=="output") {
            data.dataType = DataType::output;
            data.data = dataJson["data"];
            
        }
        
        testdata.push_back(data);
    }
    
    


}

void UnitTester::TestCase::AddInput(std::string input)
{
    Data data;
    data.data = input;
    data.dataType = DataType::input;
    testdata.push_back(std::move(data));
}


void UnitTester::TestCase::AddOutput(std::string output)
{
    Data data;
    data.data = output;
    data.dataType = DataType::input;
    testdata.push_back(std::move(data));

}


void UnitTester::AddInput(std::string in)
{
    currentSession.AddInput(in);
}
void UnitTester::AddOutput(std::string out)
{
    currentSession.AddOutput(out);
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


void UnitTester::TestAll()
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

