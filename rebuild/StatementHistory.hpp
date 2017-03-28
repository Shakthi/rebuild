//
//  StatementHistory.hpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 12/03/17.
//  Copyright © 2017 self. All rights reserved.
//

#ifndef StatementHistory_hpp
#define StatementHistory_hpp

#include "Rebuild.hpp"
#include "nlohmann/json.hpp"
#include "lineHistory.hpp"
#include "AST.hpp"

#include <string>
#include <list>

class StatementHistory:public LineHistory  {
protected:
    
    std::list<Statement*> history;
    std::list<Statement*>::iterator historyPointer;
    
    
public:
    
    void ReInit();
    void ReInitDone();

    
    StatementHistory();
   
    void Add(class Statement * st);
    bool ChekDuplicate(class Statement * st);
    void InternalAdd(class Statement * st);
    
     std::string Edit(std::string currentBuffer, MoveDirection direction,bool& success);
    
    
    const std::list<Statement*> & GetHistory() const
    {
        return history;
    }
    
     std::list<Statement*> & GetModifieableHistory()
    {
        return history;
    }
    
    void Clear();
    
    Statement *  GetCurrentStatment();
    
    
    nlohmann::json ToJson();
    void FromJson(nlohmann::json);
    ~StatementHistory(){}
};

class PopingLineStatementHistory:public StatementHistory
{
    int extracount;
public:
    PopingLineStatementHistory():extracount(0){}
    void Add(class Statement * st);
    void AddExtra(class Statement * st);
    void PopExtra();
    
};


#endif /* StatementHistory_hpp */
