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
#include <fstream>
#include <string>
#include <vector>

class StatementHistory  {
protected:
    int historyIndex;
    std::vector<std::string> history;
    
    virtual void ReInit();
    virtual  void ReInitDone();
    friend class LineNoiseWrapper;
    
public:
    enum class MoveDirection { prev,
        next };
    
    StatementHistory();
    
    void Save(std::string filename);
    void Load(std::string filename);
   void Add(class Statement * st);
    void InternalAdd(std::string entry);
    
    std::string Edit(std::string currentBuffer, MoveDirection direction,
                     bool& success);
    
    void Clear();
    
    std::string HistoryAt(int i);
    
    nlohmann::json ToJson();
    void FromJson(nlohmann::json);
};

class PopingLineStatementHistory:public StatementHistory
{
    int extracount;
public:
    PopingLineStatementHistory():extracount(0){}
    void Add(std::string entry);
    void AddExtra(std::string entry);
    void PopExtra();
    
};


#endif /* StatementHistory_hpp */
