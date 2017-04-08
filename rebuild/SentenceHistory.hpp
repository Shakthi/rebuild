//
//  SentenceHistory.hpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 12/03/17.
//  Copyright © 2017 self. All rights reserved.
//

#ifndef SentenceHistory_hpp
#define SentenceHistory_hpp

#include "Rebuild.hpp"
#include "nlohmann/json.hpp"
#include "lineHistory.hpp"
#include "AST.hpp"

#include <string>
#include <list>

class SentenceHistory:public LineHistory  {
protected:
    
    std::list<Sentence*> history;
    std::list<Sentence*>::iterator historyPointer;
    
    
public:
    
    void ReInit();
    void ReInitDone();

    
    SentenceHistory();
   
    void Add(class Sentence * st);
    bool ChekDuplicate(class Sentence * st);
    void InternalAdd(class Sentence * st);
    
     std::string Edit(std::string currentBuffer, MoveDirection direction,bool& success);
    
    
    const std::list<Sentence*> & GetHistory() const
    {
        return history;
    }
    
     std::list<Sentence*> & GetModifieableHistory()
    {
        return history;
    }
    
    void Clear();
    
    Sentence *  GetCurrentStatment();
    
    
    nlohmann::json ToJson();
    void FromJson(nlohmann::json);
    ~SentenceHistory(){}
};

class PopingLineSentenceHistory:public SentenceHistory
{
    int extracount;
public:
    PopingLineSentenceHistory():extracount(0){}
    void Add(class Sentence * st);
    void AddExtra(class Sentence * st);
    void PopExtra();
    
};


#endif /* SentenceHistory_hpp */
