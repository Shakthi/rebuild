//
//  lineHistory.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 04/02/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "lineHistory.hpp"
#include "json.hpp"
#include <ctime>



namespace  {
    
    std::string nowTime()
    {
        time_t now;
        time(&now);
        char buf[sizeof "2011-10-08T07:07:09Z"];
        strftime(buf, sizeof buf, "%FT%TZ", gmtime(&now));
        return buf;
    }
    
}

LineHistory::LineHistory():historyIndex(0)
{
}


void LineHistory::Save(std::string filename)
{
    using namespace nlohmann;
    
    json root;
    root["content"] = history;
    root["timestamp"]=nowTime();
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

    try {
        stream>>root;
        
        json content = root["content"];
        for (int i=0; i<content.size(); i++) {
            Add(content[i]);
        }
    } catch (std::invalid_argument exception) {
        
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



void LineHistory::ResetEnd()
{
    history.pop_back();

}
void LineHistory::Reset()
{
    Add("");
    historyIndex=0;
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



void Clear(std::string entry);

std::string HistoryAt(int i);
