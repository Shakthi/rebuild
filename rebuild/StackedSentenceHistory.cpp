//
//  StackedSentenceHistory.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 06/08/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "StackedSentenceHistory.hpp"




StackedSentenceHistory::StackedSentenceHistory(const std::vector<class SentenceHistory*> & stack)
:historyStack(stack),currentStackPointerInited(false)
{

    historyPointerForStack = SentenceHistory::cbegin();

}



void StackedSentenceHistory::InitHistoryStack()
{

    //stack pointer cannot be initialized
    if (!currentStackPointerInited) {
        currentStackPointer = historyStack.rbegin();
        currentStackPointerInited = true;


        SentenceHistory::InternalAdd( SentenceRef( new UnProcessedStatment));
        historyPointer = history.begin();



    }
}

std::string StackedSentenceHistory::Edit(std::string currentBuffer,
                                         MoveDirection direction,
                                         bool &success) {
    //     <---move Next ->move prevoious
    //    ABCDEFGHI | abcdef
    //    historyPointer++->
    //    currentStackPointer++->

    success = true;
    InitHistoryStack();

    if (*currentStackPointer != this) {

        if (direction == MoveDirection::prev) {
            const_iterator previousPos = historyPointerForStack;
            ++previousPos;
            if (previousPos != (*currentStackPointer)->cend()) // not yet at the end
                historyPointerForStack = previousPos;
            else {
                stack_iterator previousStack = currentStackPointer;
                previousStack++;

                if (previousStack == historyStack.rend()) {
                    success = false;
                } else {
                    currentStackPointer = previousStack;
                    historyPointerForStack = (*currentStackPointer)->cbegin();
                    assert(historyPointerForStack != (*currentStackPointer)->cend());
                }
            }

        } else if (direction == MoveDirection::next) {

            if (historyPointerForStack !=
                (*currentStackPointer)->cbegin()) // not yet at the begin
                historyPointerForStack--;
            else {

                if (currentStackPointer == historyStack.rbegin()) {
                    success = false;
                } else {
                    currentStackPointer--;

                    const_iterator lastIterotr = (*currentStackPointer)->cend();
                    assert(lastIterotr != (*currentStackPointer)->cbegin());
                    lastIterotr--;

                    historyPointerForStack = lastIterotr;

                    if (*currentStackPointer == this) {
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

            currentStackPointer++;
            assert(currentStackPointer != historyStack.rend());
            historyPointerForStack = (*currentStackPointer)->cbegin();
            assert(historyPointerForStack != (*currentStackPointer)->cend());
            result = (*historyPointerForStack)->dumpToString();
            success = true;
        }

        return result;
    }
}


void StackedSentenceHistory::Rewind()
{

    historyPointer = history.end();
    historyPointer--;

}

void StackedSentenceHistory:: EditBegin()
{
    
    
    
    // historyPointer = history.begin();
    
    
}


void StackedSentenceHistory:: EditEnd()
{
    lastStatmentIter = historyPointer;
    
}


void StackedSentenceHistory::InternalAdd(SentenceRef entry)
{
    auto prevHistoryPointer = historyPointer;
    prevHistoryPointer++;
    history.insert(prevHistoryPointer, entry);
    
}

void StackedSentenceHistory::Replace(SentenceRef entry)
{
    *historyPointer = entry;
    historyPointer--;
    
}

void StackedSentenceHistory::Add(SentenceRef entry)
{
    
    if (ChekDuplicate(entry))
    {
        InternalAdd(entry);
        
    }
    
    
    
    
    
}