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

class LineHistory {
    
    std::vector<std::string> history;
    
   
    int historyIndex;
    
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
    
    
    void Reset();
    void ResetEnd();
    void Clear(std::string entry);
    
    std::string HistoryAt(int i);
    
    
    
    
    std::string ToString();
    void FromString(std::string);
    
    
    
    
};

#endif /* lineHistory_hpp */
