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
    typedef std::list<Sentence*>::iterator iterator;
    typedef std::list<Sentence*>::const_iterator const_iterator;
    typedef std::list<Sentence*>::reverse_iterator reverse_iterator;
    std::list<Sentence*> history;
    iterator historyPointer;
    const_iterator historyPointerForStacks;
    
    
public:
    
    void ReInit();
    void ReInitDone();

    
    SentenceHistory();
   
    void Add(class Sentence * st);
    bool ChekDuplicate(class Sentence * st);
    void InternalAdd(class Sentence * st);
    
    std::string Edit(std::string currentBuffer, MoveDirection direction,bool& success);
    
    

    reverse_iterator rbegin(){ return history.rbegin();}
    reverse_iterator rend(){ return history.rend();}


    const_iterator begin(){ return history.begin();}
    const_iterator end(){ return history.end();}


    void PopHistory( );
    void Clear();
    
    Sentence *  GetCurrentStatment();
    
    
    nlohmann::json ToJson();
    void FromJson(nlohmann::json);
    ~SentenceHistory(){}
};




class PopingLineSentenceHistory:public SentenceHistory
{
    int extracount;


    const std::vector<class SentenceHistory*> & historyStack;

    std::vector<class SentenceHistory*>::const_reverse_iterator historyStackPointer;
    bool historyStackPointerInited;

public:
    PopingLineSentenceHistory(const std::vector<class SentenceHistory*> & stack)
    :extracount(0),historyStack(stack),historyStackPointerInited(false){}

    std::string Edit(std::string currentBuffer, MoveDirection direction,bool& success);


    void Add(class Sentence * st);
    void AddExtra(class Sentence * st);
    void PopExtra();
    
};


#endif /* SentenceHistory_hpp */
