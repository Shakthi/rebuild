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
#include "linenoise/lineHistory.hpp"
#include "AST.hpp"

#include <string>
#include <list>

class SentenceHistory:public LineHistory  {
protected:
    typedef std::list<SentenceRef>::iterator iterator;
    typedef std::list<SentenceRef>::const_iterator const_iterator;
    typedef std::list<SentenceRef>::reverse_iterator reverse_iterator;
    typedef std::list<SentenceRef>::const_reverse_iterator const_reverse_iterator;
    std::list<SentenceRef> history;
    iterator historyPointer;

    iterator lastStatmentIter;

    std::string currentBuffer;

public:
    void EditBegin();
    void EditEnd();

    
    SentenceHistory();
   
    void Add(SentenceRef  st);
    bool ChekDuplicate(SentenceRef  st);
    void InternalAdd(SentenceRef  st);
    
    std::string Edit(std::string currentBuffer, MoveDirection direction,bool& success);
    
    

    const_reverse_iterator rcbegin()const{ return history.rbegin();}
    const_reverse_iterator rcend()const{ return history.rend();}

    reverse_iterator rbegin(){ return history.rbegin();}
    reverse_iterator rend(){ return history.rend();}


    const_iterator cbegin()const{ return history.begin();}
    const_iterator cend()const{ return history.end();}
    iterator begin(){ return history.begin();}
    iterator end(){ return history.end();}


    void PopHistory( );
    void Splice(iterator,bool deleteStatement=true);
    void Clear();
    
    iterator  GetLastStatmentIter();
    
    
    nlohmann::json ToJson();
    void FromJson(nlohmann::json);
    ~SentenceHistory(){

       
    }
};




class StackedSentenceHistory:public SentenceHistory
{


    const std::vector<class SentenceHistory*> & historyStack;

    
    typedef typename std::vector<class SentenceHistory*>::const_reverse_iterator stack_iterator;

    stack_iterator currentStackPointer;
    SentenceHistory::const_iterator historyPointerForStack;


    bool currentStackPointerInited;
    void InitHistoryStack();




public:
    StackedSentenceHistory(const std::vector<class SentenceHistory*> & stack);

    std::string Edit(std::string currentBuffer, MoveDirection direction,bool& success);

    void InternalAdd(SentenceRef entry);


    void Add(SentenceRef);
    void EditBegin();
    void EditEnd();

    void Rewind();



    
};


#endif /* SentenceHistory_hpp */
