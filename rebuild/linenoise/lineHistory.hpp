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

class LineHistory {
public:
    virtual void EditBegin() = 0;
    virtual void EditEnd() = 0;

    enum class MoveDirection { prev,
        next , none};
    virtual std::string Edit(std::string currentBuffer, MoveDirection direction,
        bool& success)
        = 0;
};

class LineHistoryStr : public LineHistory {

protected:
    int historyIndex;
    std::vector<std::string> history;

    void EditBegin();
    void EditEnd();

public:
    LineHistoryStr();
    std::string Edit(std::string currentBuffer, MoveDirection direction,
        bool& success);

    void Save(std::string filename);
    void Load(std::string filename);
    virtual void Add(std::string entry);
    void InternalAdd(std::string entry);
    std::vector<std::string>& GetHistory();

    void Clear();

    std::string HistoryAt(int i);

    nlohmann::json ToJson();
    void FromJson(nlohmann::json);
};

class StackedLineHistoryStr : public LineHistoryStr {
    int extracount;

public:
    StackedLineHistoryStr()
        : extracount(0)
    {
    }
    void Add(std::string entry);
    void PopExtra();
};

#endif /* lineHistory_hpp */