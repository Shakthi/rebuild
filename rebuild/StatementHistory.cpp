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
: historyIndex(0)
{
}

void StatementHistory::Save(std::string filename)
{
    using namespace nlohmann;
    
    json root;
    root["content"] = history;
    root["type"] = "historyfile";
    root["creator"] = "rebuild";
    
    std::ofstream stream(filename);
    stream << root.dump(4);
    //TODO:SAVE LOAD
}

void StatementHistory::Load(std::string filename)
{
//    using namespace nlohmann;
//    std::ifstream stream(filename);
//    json root;
//    
//    stream >> root;
//    
//    json content = root["content"];
//    for (int i = 0; i < content.size(); i++) {
//        Add(content[i]);
//    }
    
//TODO:SAVE LOAD
}

void StatementHistory::Add(Statement * entry)
{
    
    
//    if (!history.empty())
//        if (*(history.rbegin()) == entry) {
//            return;
//        }
//
    
    //TODO removeduplicate
    
    InternalAdd(entry);
    
    
}

void StatementHistory::InternalAdd(Statement * entry)
{
    history.push_back(entry);
    
}

void StatementHistory::ReInit()
{
    
    
    historyIndex=0;
    InternalAdd(new UnProcessedStatment);
    
}


void StatementHistory::ReInitDone()
{
    delete history.back();
    history.pop_back();
}

std::string
StatementHistory::Edit(std::string currentBuffer, MoveDirection direction,
                     bool& success)
{
    success = true;
    if (history.size() > 1) {
        
        if(historyIndex==0)
        {
            delete history[history.size() - 1 - historyIndex];
            auto anProcessedStatment= new  UnProcessedStatment;
            anProcessedStatment->sourceText = currentBuffer;
            
            history[history.size() - 1 - historyIndex] = anProcessedStatment;
        }
        
        historyIndex += (direction == MoveDirection::prev) ? 1 : -1;
        
        if (historyIndex < 0) {
            historyIndex = 0;
            success = false;
            
        } else if (historyIndex >= history.size()) {
            historyIndex = (int)history.size() - 1;
            success = false;
        }
    }else
    {
        success = false;
    }
    
    return history[history.size() - 1 - historyIndex]->sourceText;
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
    
    for(auto  statement : history )
    {
        rootContent.push_back(statement->ToJson());
    }
    
    root["content"] = rootContent;
    
    
    
    
    root["type"] = "historyfile";
    root["creator"] = "rebuild";
    
    
    
    return root;
}
void StatementHistory::FromJson(nlohmann::json root)
{
//    using namespace nlohmann;
//    
//    json content = root["content"];
//    for (int i = 0; i < content.size(); i++) {
//        Add(content[i]);
//    }
}


void PopingLineStatementHistory::PopExtra()
{
    for (int i=0; i<extracount; i++)
    {
        delete history.back();
        history.pop_back();
        if(historyIndex>0)
            historyIndex--;
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
    if (!history.empty())
    {
//        if (*(history.rbegin()) == entry) {
//            return;
//        }
    }
    
    for (int i=0; i<historyIndex; i++)
    {
        delete history.back();
        history.pop_back();
    }
    InternalAdd(entry);
    
    
}