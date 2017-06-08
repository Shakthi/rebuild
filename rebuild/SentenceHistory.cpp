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



bool SentenceHistory::ChekDuplicate( Sentence * entry)
{
    
        if (!history.empty())
            if (history.front()->isEqual(*entry) ) {
                return false;
            }
    
    return true;
}

void SentenceHistory::Add(Sentence * entry)
{
    
    
    if(ChekDuplicate(entry))
    {
        InternalAdd(entry);
    }
    
}

void SentenceHistory::InternalAdd(Sentence * entry)
{
    history.push_front(entry);
    
}

void SentenceHistory::ReInit()
{
    
    
    
    InternalAdd(new UnProcessedStatment);
    historyPointer = history.begin();

    
}


void SentenceHistory::ReInitDone()
{
    if(historyPointer == history.begin())
        historyPointer++;
        
        
    delete history.front();
    history.pop_front();

}

std::string
SentenceHistory::Edit(std::string currentBuffer, MoveDirection direction,
                     bool& success)
{
    
    
    success = true;//current buffer is changed
    
    assert(!history.empty());
    
    if(history.size() == 1) {  //No other element than current line
        success = false;
        return currentBuffer;
    }
    
    

    //save current buffer only if it is at the begning
    if( historyPointer == history.begin())
    {
        delete *historyPointer;
        auto anProcessedStatment = new  UnProcessedStatment;
        anProcessedStatment->sourceText = currentBuffer;
        *historyPointer = anProcessedStatment;
    }
        
        
    
    if (direction == MoveDirection::next) {
        
        if (historyPointer != history.begin())
            historyPointer --;
        else
            success = false;
        
    } else {
        
        historyPointer ++;
        if (historyPointer == history.end()) {
            success = false;
            historyPointer --;
        }
    }
        
        
    
   
    
    return (*historyPointer)->dumpToString();
}

Sentence *  SentenceHistory::GetCurrentStatment()
{
    return *historyPointer;
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
        
        InternalAdd(Sentence::GetFromJson(content[content.size()-i-1]));
        
    }
}


//void Pop()
void SentenceHistory::PopHistory()
{
    if (!history.empty()) {
        history.erase(history.begin());
    }


}

void PopingLineSentenceHistory::PopExtra()
{
    for (int i=0; i<extracount; i++)
    {
        if(historyPointer== history.begin())
            historyPointer++;
        
        delete history.front();
        history.pop_front();
        
            
    }
    extracount=0;
    
    
}

void PopingLineSentenceHistory::AddExtra(Sentence * entry)
{
    InternalAdd(entry);
    extracount++;
    
}

std::string PopingLineSentenceHistory::Edit(std::string currentBuffer, MoveDirection direction,bool& success)
{
    if(!historyStackPointerInited){
        historyStackPointer =historyStack.rbegin();
        historyStackPointerInited = true;
    }

    if(*historyStackPointer == this){


    success = true;//current buffer is changed

    assert(!history.empty());

    if(history.size() == 1) {  //No other element than current line
        success = false;
        return currentBuffer;
    }



    //save current buffer only if it is at the begning
    if( historyPointer == history.begin())
    {
        delete *historyPointer;
        auto anProcessedStatment = new  UnProcessedStatment;
        anProcessedStatment->sourceText = currentBuffer;
        *historyPointer = anProcessedStatment;
    }



    if (direction == MoveDirection::next) {

        if (historyPointer != history.begin())
            historyPointer --;
        else
            success = false;

    } else {

        historyPointer ++;
        if (historyPointer == history.end()) {

            historyStackPointer++;
            historyPointerForStacks = (*historyStackPointer)->begin();
            return (*historyPointerForStacks)->dumpToString();
        }
    }


    return (*historyPointer)->dumpToString();
    }
    else return "";

}

void PopingLineSentenceHistory::Add(Sentence * entry)
{
  

    
    if (ChekDuplicate(entry))
    {
        InternalAdd(entry);
        
    }
    
   
    
    
    
}