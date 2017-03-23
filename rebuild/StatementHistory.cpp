//
//  StatementHistory.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 12/03/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "StatementHistory.hpp"
#include "AST.hpp"


StatementHistory::StatementHistory()
{
    historyPointer = history.rbegin();
    

}



bool StatementHistory::ChekDuplicate(Statement * entry)
{
    
        if (!history.empty())
            if (history.back()->dumpToString() == entry->dumpToString()) {
                return false;
            }
    
    return true;
}

void StatementHistory::Add(Statement * entry)
{
    
    
    if(ChekDuplicate(entry))
    {
        InternalAdd(entry);
    }
    
}

void StatementHistory::InternalAdd(Statement * entry)
{
    history.push_back(entry);
    
}

void StatementHistory::ReInit()
{
    
    
    
    InternalAdd(new UnProcessedStatment);
    historyPointer = history.rbegin();

    
}


void StatementHistory::ReInitDone()
{
    delete history.back();
    history.pop_back();
    historyPointer = history.rbegin();

}

std::string
StatementHistory::Edit(std::string currentBuffer, MoveDirection direction,
                     bool& success)
{
    
    
    success = true;//current buffer is changed
    
    assert(!history.empty());
    
    if(history.size() == 1) {  //No other element than current line
        success =false;
        return currentBuffer;
    }
    
    
    
    
    


    //save current buffer only if it is at the begning
    if( historyPointer == history.rbegin())
    {
        delete history.back();
        auto anProcessedStatment= new  UnProcessedStatment;
        anProcessedStatment->sourceText = currentBuffer;
        history.back() = anProcessedStatment;
    }
        
        
    
    if (direction == MoveDirection::next) {
        
        if (historyPointer != history.rbegin())
            historyPointer --;
        else
            success = false;
        
    } else {
        
        historyPointer ++;
        if (historyPointer == history.rend()) {
            success = false;
            historyPointer --;
        }
    }
        
        
    
   
    
    return (*historyPointer)->dumpToString();
}

void StatementHistory::Clear()
{
    history.clear();
}



nlohmann::json
StatementHistory::ToJson()
{
    using namespace nlohmann;
    
    json root;
    json rootContent;
    
 
    //history return in inverse cronographic order
    for (auto iter =history.rbegin(); iter!=history.rend(); iter++) {
        rootContent.push_back((*iter)->ToJson());
    }
    
    
    
    root["content"] = rootContent;
    
    root["version"] = 2;
    
    
    
    
    root["type"] = "historyfile";
    root["creator"] = "rebuild";
    
    
    
    return root;
}
void StatementHistory::FromJson(nlohmann::json root)
{
    using namespace nlohmann;
    
    json content = root["content"];
    
    assert(root["version"].get<int>() == 2);
    
    for (int i = 0; i < content.size(); i++) {
        
        InternalAdd(Statement::GetFromJson(content[content.size()-i-1]));
        
    }
}


void PopingLineStatementHistory::PopExtra()
{
    for (int i=0; i<extracount; i++)
    {
        delete history.back();
        history.pop_back();
        if(historyPointer!= history.rbegin())
            historyPointer--;
    }
    extracount=0;
    
    
}

void PopingLineStatementHistory::AddExtra(Statement * entry)
{
    InternalAdd(entry);
    extracount++;
    
}


void PopingLineStatementHistory::Add(Statement * entry)
{
    
    while (historyPointer == history.rbegin()) {
        delete history.back();
        history.pop_back();
    }
    
    
    
    if (ChekDuplicate(entry))
    {
        InternalAdd(entry);
        
    }
    
   
    
    
    
}