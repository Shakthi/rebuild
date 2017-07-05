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
    enum class ExitStatus {
        ok,//Normal exit with result
        ctrl_c,//Exit pressing key ctrl-c : exit operation
        ctrl_d,//Exit pressing key ctrl-d : end of buffer
        ctrl_X//Any other cotrol keys
        };
 
    enum class EModificationStatus {
        ok,//User niether pressed edit keys or moved through history
        history,//User selected inout from history
        edited,//User edited the buffer
    };

    


    struct ExtraResults {

        ExitStatus status;
        EModificationStatus mstatus;
        int ctrlKey;
    };



    std::string filter;

private:
    EModificationStatus mstatus;



public:
    void SetHistoryFilter(std::string filter);//Scroll via only with those
    std::string getLine(std::string prompt);
    std::string getLineWithHistory(std::string prompt,LineHistory & inhistory,ExtraResults  & extraResults);
    
    ~LineNoiseWrapper();
    LineNoiseWrapper(LineHistory & linehistory);
    
    
    static  char* linenoiseHistoryCallbackStatic(int direction,
        const char* oldline,
        void* context,size_t * cursorPos);

    static char* LinenoiseHistoryHintCallbackStatic(const char *, int *color, int *bold);
    static void LinenoiseHistoryFreeHintsCallback(void*);

    
    std::string LinenoiseHistoryCallback(int movedirection,std::string oldline, bool & successStatus );


    

    nlohmann::json ToJson();
    void FromJson(nlohmann::json);
    void ClearScreen();
};
#endif /* lineNoiseWrapper_hpp */
