//
//  lineHistory.hpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 04/02/17.
//  Copyright © 2017 self. All rights reserved.
//

#ifndef lineHistory_hpp
#define lineHistory_hpp

#include <string>
#include <vector>
#include <fstream>
#include "json.hpp"
#include "Rebuild.hpp"


class LineHistory : public Rebuild::Serialised {
    
    std::vector<std::string> history;
    
    
    int historyIndex;
    
    void ReInit();
    void ReInitDone();
    friend class LineNoiseWrapper;
    
    
public:
    enum class MoveDirection
    {
        prev,
        next
    };
    
    
    LineHistory();
    
    void Save(std::string filename);
    void Load(std::string filename);
    void Add(std::string entry);
    std::string Edit(std::string currentBuffer,MoveDirection direction, bool & success);
    
    
    
    
    
    void Clear();
    
    std::string HistoryAt(int i);
    
    
    
    
    nlohmann::json ToJson();
    void FromJson(nlohmann::json);
    
    
    
    
};

#endif /* lineHistory_hpp */
