//
//  SentenceHistory.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 12/03/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "SentenceHistory.hpp"
#include "AST.hpp"
#include "Logger.hpp"


SentenceHistory::SentenceHistory()
{
    historyPointer = history.begin();


}



bool SentenceHistory::ChekDuplicate( SentenceRef entry)
{

    if (!history.empty())
        if (history.front()->isEqual(*entry) ) {
            return false;
        }

    return true;
}

void SentenceHistory::Add(SentenceRef entry)
{


    if(ChekDuplicate(entry))
    {
        InternalAdd(entry);
    }

}

void SentenceHistory::InternalAdd(SentenceRef entry)
{
    history.push_front(entry);

}

void SentenceHistory:: EditBegin()
{


    

    InternalAdd( SentenceRef( new UnProcessedStatment));
    historyPointer = history.begin();
    


}


 bool SentenceHistory:: IsAtBeginPosition()
{
    return historyPointer == history.begin();
}


void SentenceHistory:: EditEnd()
{
    if(!IsAtBeginPosition())
    {
        lastStatmentIter = historyPointer;

    }else
    {
        historyPointer++;
        lastStatmentIter = history.end();
    }





    history.pop_front();

}

std::string
SentenceHistory::Edit(std::string currentBuffer, MoveDirection direction,
                      bool& success)
{


    success = true;//current buffer is changed

    assert(!history.empty());







    //save current buffer only if it is at the begning
    if( IsAtBeginPosition() && direction != MoveDirection::none)
    {
        this->currentBuffer = currentBuffer;
    }



    if (direction == MoveDirection::next) {

        if (historyPointer != history.begin())
            historyPointer --;
        else
            success = false;

    } else if (direction == MoveDirection::prev) {

        historyPointer ++;
        if (historyPointer == history.end()) {
            success = false;
            historyPointer --;
        }
    }else if (direction == MoveDirection::none) {

        success = true;

    }



    if( IsAtBeginPosition() && direction != MoveDirection::none)
    {
        return  this->currentBuffer;

    }else{

        return (*historyPointer)->dumpToString();
    }

}

SentenceHistory::iterator  SentenceHistory::GetLastStatmentIter()
{
    return lastStatmentIter;
}

void SentenceHistory::Clear()
{
    history.clear();
}



nlohmann::json
SentenceHistory::ToJson()
{
    using namespace nlohmann;

    json root;
    json rootContent;



    for (auto iter =history.begin(); iter!=history.end(); iter++) {
        rootContent.push_back((*iter)->ToJson());
    }



    root["content"] = rootContent;

    root["version"] = 2;




    root["type"] = "historyfile";
    root["creator"] = "rebuild";

    return root;
}



void SentenceHistory::FromJson(nlohmann::json root)
{
    using namespace nlohmann;

    json content = root["content"];

    assert(root["version"].get<int>() == 2);

    for (int i = 0; i < content.size(); i++) {

        InternalAdd(SentenceRef( Sentence::GetFromJson(content[content.size()-i-1])));

    }
}


//void Pop()
void SentenceHistory::Splice(SentenceHistory::iterator iter,bool deleteStatement)
{
    if (!history.empty()) {
    
        history.erase(iter);

    }
}


void SentenceHistory::PopHistory()
{
    Splice(history.begin());

    
}
