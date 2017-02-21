//
//  lineHistory.hpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 04/02/17.
//  Copyright © 2017 self. All rights reserved.
//

#ifndef lineHistory_hpp
#define lineHistory_hpp

#include "Rebuild.hpp"
#include "json.hpp"
#include <fstream>
#include <string>
#include <vector>

class LineHistory : public Rebuild::Serialised {

protected:
    int historyIndex;
    std::vector<std::string> history;

    virtual void ReInit();
    virtual  void ReInitDone();
    friend class LineNoiseWrapper;

public:
    enum class MoveDirection { prev,
        next };

    LineHistory();

    void Save(std::string filename);
    void Load(std::string filename);
    virtual void Add(std::string entry);
    void InternalAdd(std::string entry);
    std::vector<std::string> & GetHistory();
    
    std::string Edit(std::string currentBuffer, MoveDirection direction,
        bool& success);

    void Clear();

    std::string HistoryAt(int i);

    nlohmann::json ToJson();
    void FromJson(nlohmann::json);
};

class PopingLineHistory:public LineHistory
{
    int extracount;
public:
    PopingLineHistory():extracount(0){}
    void Add(std::string entry);
    void AddExtra(std::string entry);
    void PopExtra();

};


#endif /* lineHistory_hpp */
