//
//  StackedSentenceHistory.hpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 06/08/17.
//  Copyright © 2017 self. All rights reserved.
//

#ifndef StackedSentenceHistory_hpp
#define StackedSentenceHistory_hpp

#include "SentenceHistory.hpp"



class StackedSentenceHistory:public SentenceHistory
{


    const std::vector<class SentenceHistory*> & historyStack;


    typedef typename std::vector<class SentenceHistory*>::const_reverse_iterator stack_iterator;

    stack_iterator currentStackPointer;
    SentenceHistory::const_iterator historyPointerForStack;


    bool currentStackPointerInited;
    void InitHistoryStack();


    iterator historyWritePointer;
    bool IsAtBeginPosition();


public:
    StackedSentenceHistory(const std::vector<class SentenceHistory*> & stack);

    std::string Edit(std::string currentBuffer, MoveDirection direction,bool& success);

    void InternalAdd(SentenceRef entry);


    void Add(SentenceRef);
    void Replace(SentenceRef);

    void EditBegin();
    void EditEnd();
    
    void Rewind();
    
    
    
    
};


#endif /* StackedSentenceHistory_hpp */
