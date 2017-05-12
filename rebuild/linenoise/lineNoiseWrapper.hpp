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
    enum class ExitStatus { ok,
        ctrl_c,
        ctrl_d,
        ctrl_X
        };
 
    enum class EModificationStatus { ok,
        history,
        edited,
    };

    

private:
    ExitStatus status;
    EModificationStatus mstatus;
    int ctrlKey;
    
    std::string loadedBuffer;
    std::string filter;
    
    

public:
    void SetHistoryFilter(std::string filter);//Scroll via only with those
    std::string getLine(std::string prompt);
    std::string getLineWithHistory(std::string prompt,LineHistory & inhistory);
    ExitStatus GetStatus() { return status; }
    EModificationStatus GetModificationStatus() { return mstatus; }
    int GetControlKey(){return ctrlKey;}

    ~LineNoiseWrapper();
    LineNoiseWrapper(LineHistory & linehistory);
    
    
    static  char* linenoiseHistoryCallbackStatic(int direction,
        const char* oldline,
        void* context,size_t * cursorPos);
    
    
    std::string LinenoiseHistoryCallback(int movedirection,std::string oldline, bool & successStatus );
    
    

    nlohmann::json ToJson();
    void FromJson(nlohmann::json);
    void ClearScreen();
};
#endif /* lineNoiseWrapper_hpp */
