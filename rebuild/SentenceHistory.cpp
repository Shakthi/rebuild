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
    {
        historyPointer++;
        lastStatmentIter = history.end();
    }else
    {
        lastStatmentIter = historyPointer;
    }





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

        InternalAdd(Sentence::GetFromJson(content[content.size()-i-1]));

    }
}


//void Pop()
void SentenceHistory::Splice(SentenceHistory::iterator iter)
{
    if (!history.empty()) {
        delete *iter;
        history.erase(iter);

    }
}


void SentenceHistory::PopHistory()
{
    if (!history.empty()) {
        delete *history.begin();
        history.erase(history.begin());
    }

    
}


PopingLineSentenceHistory::PopingLineSentenceHistory(const std::vector<class SentenceHistory*> & stack)
:historyStack(stack),stackPointerInited(false)
{

    historyPointerForStack = SentenceHistory::cbegin();


}



void PopingLineSentenceHistory::InitHistoryStack()
{

    //stack pointer cannot be initialized
    if (!stackPointerInited) {
        stackPointer = historyStack.rbegin();
        stackPointerInited = true;
    }
}

std::string PopingLineSentenceHistory::Edit(std::string currentBuffer,
                                            MoveDirection direction,
                                            bool &success) {
  //     <---move Next ->move prevoious
  //    ABCDEFGHI | abcdef
  //    historyPointer++->
  //    stackPointer++->

  success = true;
  InitHistoryStack();

  if (*stackPointer != this) {

    if (direction == MoveDirection::prev) {
      const_iterator previousPos = historyPointerForStack;
      ++previousPos;
      if (previousPos != (*stackPointer)->cend()) // not yet at the end
        historyPointerForStack = previousPos;
      else {
        stack_iterator previousStack = stackPointer;
        previousStack++;

        if (previousStack == historyStack.rend()) {
          success = false;
        } else {
          stackPointer = previousStack;
          historyPointerForStack = (*stackPointer)->cbegin();
          assert(historyPointerForStack != (*stackPointer)->cend());
        }
      }

    } else if (direction == MoveDirection::next) {

      if (historyPointerForStack !=
          (*stackPointer)->cbegin()) // not yet at the begin
        historyPointerForStack--;
      else {

        if (stackPointer == historyStack.rbegin()) {
          success = false;
        } else {
          stackPointer--;

          const_iterator lastIterotr = (*stackPointer)->cend();
          assert(lastIterotr != (*stackPointer)->cbegin());
          lastIterotr--;

          historyPointerForStack = lastIterotr;

          if (*stackPointer == this) {
            iterator nextHistoryPointer = history.end();
            nextHistoryPointer--;
            historyPointer = nextHistoryPointer;
          }
        }
      }
    }

    return (*historyPointerForStack)->dumpToString();

  } else {

    std::string result =
        SentenceHistory::Edit(currentBuffer, direction, success);
    if (!success && direction == MoveDirection::prev) {

      stackPointer++;
      assert(stackPointer != historyStack.rend());
      historyPointerForStack = (*stackPointer)->cbegin();
      assert(historyPointerForStack != (*stackPointer)->cend());
      result = (*historyPointerForStack)->dumpToString();
      success = true;
    }

    return result;
  }
}



void PopingLineSentenceHistory::Add(Sentence * entry)
{
    
    
    
    if (ChekDuplicate(entry))
    {
        InternalAdd(entry);
        
    }
    
    
    
    
    
}