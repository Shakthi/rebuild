//
//  lineHistory.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 04/02/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "lineHistory.hpp"
#include <ctime>
#include <sstream>



LineHistory::LineHistory():historyIndex(0)
{
}



void LineHistory::Save(std::string filename)
{
    using namespace nlohmann;
    
    json root;
    root["content"] = history;
    root["type"]="historyfile";
    root["creator"]="rebuild";
    
    
    
    std::ofstream stream(filename);
    stream<<root.dump(4);
    
}


void LineHistory::Load(std::string filename)
{
    using namespace nlohmann;
    std::ifstream stream(filename);
    json root;
    
    stream>>root;
    
    
    json content = root["content"];
    for (int i=0; i<content.size(); i++) {
        Add(content[i]);
    }
    
    
}



void LineHistory::Add(std::string entry)
{
    if(!history.empty())
        if (*(history.rbegin()) == entry) {
            return;
        }
    
    history.push_back(entry);
}



void LineHistory::ReInit()
{
    Add("");
    historyIndex=0;
    
}
void LineHistory::ReInitDone()
{
    history.pop_back();
    
}




std::string LineHistory::Edit(std::string currentBuffer,MoveDirection direction,bool & success)
{
    
    success =true;
    if(history.size()>1) {
        
        history[history.size()-1 - historyIndex] = currentBuffer;
        historyIndex += (direction == MoveDirection::prev) ? 1 : -1;
        
        if (historyIndex < 0) {
            
            historyIndex = 0;
            success =false;
            
        } else if (historyIndex >= history.size()) {
            
            historyIndex = (int)history.size()-1;
            success =false;
        }
        
        
    }
    
    return history[history.size() - 1 - historyIndex];
    
}


void LineHistory::Clear()
{
    history.clear();
}

std::string LineHistory::HistoryAt(int i)
{
    return history[history.size() - 1 - i];
    
}




nlohmann::json LineHistory::ToJson()
{
    using namespace nlohmann;
    
    json root;
    root["content"] = history;
    root["type"]="historyfile";
    root["creator"]="rebuild";
    
    
    
    return root;
    
}
void LineHistory::FromJson(nlohmann::json root)
{
    using namespace nlohmann;
    
    
    json content = root["content"];
    for (int i=0; i<content.size(); i++) {
        Add(content[i]);
    }
    
}

