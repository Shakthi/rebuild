//
//  lineHistory.hpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 04/02/17.
//  Copyright © 2017 self. All rights reserved.
//

#ifndef lineHistory_hpp
#define lineHistory_hpp


#include "nlohmann/json.hpp"
#include <fstream>
#include <string>
#include <vector>


class LineHistory
{
public:
    
    virtual void ReInit();
    virtual  void ReInitDone();
    friend class LineNoiseWrapper;

    enum class MoveDirection { prev,
        next };
    virtual std::string Edit(std::string currentBuffer, MoveDirection direction,
                     bool& success) =0;
    virtual void Save(std::string filename)=0;
    virtual void Load(std::string filename)=0;
    
    virtual nlohmann::json ToJson();
    virtual void FromJson(nlohmann::json);


};

class LineHistoryStr:public LineHistory  {

protected:
    int historyIndex;
    std::vector<std::string> history;

    void ReInit();
    void ReInitDone();
public:
    

    LineHistoryStr();

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
    virtual void AddExtra(std::string entry)=0;
    virtual void PopExtra()=0;
    
};

class PopingLineHistoryStr:public LineHistoryStr
{
    int extracount;
public:
    PopingLineHistoryStr():extracount(0){}
    void Add(std::string entry);
    void AddExtra(std::string entry);
    void PopExtra();

};


#endif /* lineHistory_hpp */
