//
//  StackedSentenceHistory.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 06/08/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "StackedSentenceHistory.hpp"

StackedSentenceHistory::StackedSentenceHistory(const std::vector<class SentenceHistory*>& stack)
    : historyStack(stack)
    , currentStackPointerInited(false)
{

    historyPointerForStack = SentenceHistory::cbegin();
}

void StackedSentenceHistory::InitHistoryStack()
{

    //stack pointer cannot be initialized
    if (!currentStackPointerInited) {

        currentStackPointer = historyStack.size() - 1;
        currentStackPointerInited = true;
        SentenceHistory::InternalAdd(SentenceRef(new UnProcessedStatment));
        historyWritePointer = history.begin();
    }
}

std::string StackedSentenceHistory::Edit(std::string currentBuffer,
    MoveDirection direction,
    bool& success)
{
    //     <---move Next ->move prevoious
    //    ABCDEFGHI | abcdef
    //    historyPointer++->
    //    <----currentStackPointer--

    success = true;

    if (historyStack[currentStackPointer] == this) {

        std::string result = SentenceHistory::Edit(currentBuffer, direction, success);
        if (success)
            return result; //All good this just normal history browsing in current stack

        //If we are moving above the current stack
        if (direction == MoveDirection::prev) {

            assert(currentStackPointer > 0);

            currentStackPointer--;
            historyPointerForStack = (historyStack[currentStackPointer])->cbegin();

            assert(historyPointerForStack != (historyStack[currentStackPointer])->cend());

            result = (*historyPointerForStack)->dumpToString();
            success = true;
        }

        return result;
    }

    //We are scrolling over previous stack

    if (direction == MoveDirection::prev) {
        auto previousHistoryPointerForStack = historyPointerForStack;
        ++previousHistoryPointerForStack;
        if (previousHistoryPointerForStack != (historyStack[currentStackPointer])->cend()) {
            // not yet at the end

            historyPointerForStack = previousHistoryPointerForStack;

        } else {

            if (currentStackPointer > 0) {

                currentStackPointer--;
                historyPointerForStack = (historyStack[currentStackPointer])->cbegin();
                assert(historyPointerForStack != (historyStack[currentStackPointer])->cend());

            } else {

                success = false;
            }
        }

    } else if (direction == MoveDirection::next) {

        if (historyPointerForStack != (historyStack[currentStackPointer])->cbegin()) {

            // not yet at the begin
            historyPointerForStack--;

        } else {

            if (currentStackPointer < historyStack.size()) {

                currentStackPointer++;

                auto nextHistoryPointerForStack = (historyStack[currentStackPointer])->cend();
                assert(nextHistoryPointerForStack != (historyStack[currentStackPointer])->cbegin());
                nextHistoryPointerForStack--;

                historyPointerForStack = nextHistoryPointerForStack;

                if (historyStack[currentStackPointer] == this) {

                    iterator nextHistoryPointer = history.end();
                    nextHistoryPointer--;
                    historyPointer = nextHistoryPointer;
                }
            } else {

                success = false;
            }
        }
    }

    return (*historyPointerForStack)->dumpToString();
}

bool StackedSentenceHistory::IsAtBeginPosition()
{
    return historyWritePointer == historyPointer;
}

void StackedSentenceHistory::Rewind()
{

    historyWritePointer = history.end();
    historyWritePointer--;
    historyPointer = historyWritePointer;
}

void StackedSentenceHistory::PopBack()
{

    historyWritePointer++;
    historyPointer = historyWritePointer;
}

void StackedSentenceHistory::EditBegin()
{


    InitHistoryStack();

    historyPointer = historyWritePointer;
}

void StackedSentenceHistory::EditEnd()
{
    lastStatmentIter = historyPointer;
}

const StackedSentenceHistory::iterator StackedSentenceHistory::GetHistoryWritePointer()
{
    return historyWritePointer;
}

void StackedSentenceHistory::InternalAdd(SentenceRef entry)
{
    auto prevHistoryPointer = historyWritePointer;
    prevHistoryPointer++;
    history.insert(prevHistoryPointer, entry);
}

void StackedSentenceHistory::Replace(SentenceRef entry)
{
    *historyWritePointer = entry;
    historyWritePointer--;

    historyPointer = historyWritePointer;
}

void StackedSentenceHistory::Add(SentenceRef entry)
{
    InitHistoryStack(); //Need to when prepaopulating

    if (ChekDuplicate(entry)) {
        InternalAdd(entry);
    }
}