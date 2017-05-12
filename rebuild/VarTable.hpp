//
//  VarTable.hpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 08/05/17.
//  Copyright © 2017 self. All rights reserved.
//

#ifndef VarTable_hpp
#define VarTable_hpp
#include <map>
#include <string>
#include "nlohmann/json.hpp"
class Value;

class VarTable {


     std::map<std::string,Value> table;
    VarTable * super;

public:
    VarTable();
    Value & GetVar(std::string varName);
    const Value & GetValue(std::string varName);

    void SetVar(std::string varName, Value && val);
    void SetSuper(VarTable * vt);

    nlohmann::json ToJson();
    void FromJson(nlohmann::json root);

    void Clear(){ table.clear(); }

};


#endif /* VarTable_hpp */
