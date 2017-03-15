//
//  lineNoiseWrapper.hpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 31/01/17.
//  Copyright © 2017 self. All rights reserved.
//

#ifndef lineNoiseWrapper_hpp
#define lineNoiseWrapper_hpp

#include "lineHistory.hpp"
#include "Rebuild.hpp"
#include <string>

class LineNoiseWrapper  {
    LineHistory & defaultHistory;
    LineHistory * localHistory;
    

public:
    enum class EStatus { ok,
        ctrl_c,
        ctrl_d };

private:
    EStatus status;

public:
    std::string getLine(std::string prompt);
    std::string getLineWithHistory(std::string prompt,LineHistory & inhistory);
    EStatus GetStatus() { return status; }
    ~LineNoiseWrapper();
    LineNoiseWrapper(LineHistory & linehistory);
    
    
    static  char* linenoiseHistoryCallbackStatic(int direction,
        const char* oldline,
        void* context);
    
    
    std::string LinenoiseHistoryCallback(int movedirection,std::string oldline, bool & successStatus );
    
    

    nlohmann::json ToJson();
    void FromJson(nlohmann::json);
};
#endif /* lineNoiseWrapper_hpp */
