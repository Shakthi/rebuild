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

LineHistoryStr::LineHistoryStr()
    : historyIndex(0)
{
}

void LineHistoryStr::Save(std::string filename)
{
    using namespace nlohmann;

    json root;
    root["content"] = history;
    root["type"] = "historyfile";
    root["creator"] = "rebuild";

    std::ofstream stream(filename);
    stream << root.dump(4);
}

void LineHistoryStr::Load(std::string filename)
{
    using namespace nlohmann;
    std::ifstream stream(filename);
    json root;

    stream >> root;

    json content = root["content"];
    for (int i = 0; i < content.size(); i++) {
        Add(content[i]);
    }
}

void LineHistoryStr::Add(std::string entry)
{
    if (!history.empty())
        if (*(history.rbegin()) == entry) {
            return;
        }
    
    InternalAdd(entry);
    

}

void LineHistoryStr::InternalAdd(std::string entry)
{
    history.push_back(entry);

}

void LineHistoryStr:: EditBegin()
{
    
    
    historyIndex=0;
    InternalAdd("");
    
}
    

void LineHistoryStr:: EditEnd()
{
    history.pop_back();
}

std::string
LineHistoryStr::Edit(std::string currentBuffer, MoveDirection direction,
    bool& success)
{
    success = true;
    if (history.size() > 1) {
        history[history.size() - 1 - historyIndex] = currentBuffer;
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

    return history[history.size() - 1 - historyIndex];
}

void LineHistoryStr::Clear()
{
    history.clear();
}

std::string
LineHistoryStr::HistoryAt(int i)
{
    return history[history.size() - 1 - i];
}

nlohmann::json
LineHistoryStr::ToJson()
{
    using namespace nlohmann;

    json root;
    root["content"] = history;
    root["type"] = "historyfile";
    root["creator"] = "rebuild";

    return root;
}
void LineHistoryStr::FromJson(nlohmann::json root)
{
    using namespace nlohmann;

    json content = root["content"];
    for (int i = 0; i < content.size(); i++) {
        Add(content[i]);
    }
}

std::vector<std::string> & LineHistoryStr::GetHistory()
{
    return history;
}


void StackedLineHistoryStr::PopExtra()
{
    for (int i=0; i<extracount; i++)
    {
        history.pop_back();
        if(historyIndex>0)
            historyIndex--;
    }
    extracount=0;
    
    
}


void StackedLineHistoryStr::Add(std::string entry)
{
    if (!history.empty())
        if (*(history.rbegin()) == entry) {
            return;
        }
    
    for (int i=0; i<historyIndex; i++)
        history.pop_back();
    InternalAdd(entry);


}