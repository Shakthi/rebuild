//
//  VarTable.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 08/05/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "VarTable.hpp"
#include "quickbasic.h"

VarTable::VarTable():super(NULL)
{}

Value & VarTable::GetVar(std::string varName)
{
    assert(varName!="");

    //auto currentTable = &table;

    return table[varName];

}
void VarTable::SetVar(std::string varName, Value && val)
{
    table[varName] = val;
}
void VarTable::SetSuper(VarTable * vt)
{
    assert(vt!=nullptr);
    super = vt;
}

nlohmann::json VarTable::ToJson()
{

    nlohmann::json j;

    for (auto varaible : table) {
        if (varaible.second.valutype == Value::Evaluetype::floattype) {
            nlohmann::json item;
            item["key"] = varaible.first;
            item["value"] = varaible.second.getNumVal();
            j.push_back(item);
        } else if (varaible.second.valutype == Value::Evaluetype::stringtype) {
            nlohmann::json item;
            item["key"] = varaible.first;
            item["value"] = varaible.second.getStringVal();
            j.push_back(item);
        }
    }




    return j;

}
void VarTable::FromJson(nlohmann::json j)
{
    nlohmann::json varablelist = j;
    for (nlohmann::json item : varablelist) {
        if (item["value"].is_number()) {
            table[item["key"]] = Value((float)item["value"]);

        } else if (item["value"].is_string()) {

            table[item["key"]] = Value(item["value"].get<std::string>());
        }
    }




}


VarTable varTable;
