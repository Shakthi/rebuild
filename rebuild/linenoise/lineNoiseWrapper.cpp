//

//  lineNoiseWrapper.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 31/01/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "lineNoiseWrapper.hpp"
#include "linenoise.h"
#include <stdlib.h>




std::string LineNoiseWrapper::getLineWithHistory(std::string prompt,LineHistory & inhistory,ExtraResults  & extraResults,const  std::string & prefilled)
{
    localHistory = &inhistory;
    
    std::string returnstring;
    inhistory.EditBegin();
    
    linenoiseOptions option;
    linenoiseOptionsInitDefaults(&option);
    linenoiseResults results;

    bool success;
    const std::string & defualtHistory  = inhistory.Edit("", LineHistory::MoveDirection::none, success);
    if(success)
        option.preFilledInput = defualtHistory.c_str();
    else
        option.preFilledInput = prefilled.c_str();


    results.ctrlKey=0;
    results.printNewln = false;

    option.printNewln = false;
    extraResults.ctrlKey =0;
    mstatus = EModificationStatus::ok;
    
    char* result = linenoise(prompt.c_str(),&option,&results);
    inhistory.EditEnd();
    
    if(option.printNewln==false && results.printNewln == false)
        printf("\n");
    
    
    if (result) {
        returnstring = result;
    }
    
    
    if (errno == EAGAIN)
        extraResults.status = ExitStatus::ctrl_c;
    
    else if (results.ctrlKey!=0)
    {
        extraResults.status = ExitStatus::ctrl_X;
        extraResults.ctrlKey=results.ctrlKey;
    }
    else{
        extraResults.status = ExitStatus::ok;

    }

        
    
    if(results.edited)
        extraResults.mstatus = EModificationStatus::edited;
    else
        extraResults.mstatus =  mstatus;


    
    linenoiseFree(result);
    
    return returnstring;
    
}



///Returns the line read by prompt with default history
std::string
LineNoiseWrapper::getLine(std::string prompt)
{
    ExtraResults extraResults;
    return getLineWithHistory(prompt, defaultHistory,extraResults);
}


void LineNoiseWrapper::SetHistoryFilter(std::string filter){
    this->filter = filter;
}

void LineNoiseWrapper::LinenoiseHistoryFreeHintsCallback(void * ptr)
{
    free(ptr);
}
 char* LineNoiseWrapper::LinenoiseHistoryHintCallbackStatic(const char * buf, int *color, int *bold)
{
//    *color=1;
//    *bold=1;
//    if (buf[0]==0) {
//        *color = 35;
//        *bold = 0;
//        return strdup("World");
//    }
    return NULL;
}

 char*
LineNoiseWrapper::linenoiseHistoryCallbackStatic(int direction, const char* oldline,
    void* context, size_t * cursorPos)
{
    std::string filtered;
    if(*cursorPos!=strlen(oldline))
        filtered= std::string(oldline).substr(0,*cursorPos+1);
    else
        *cursorPos=-1;




    LineNoiseWrapper * that= static_cast<LineNoiseWrapper*>(context);

    that->SetHistoryFilter(filtered);
    bool success;
    std::string result = that->LinenoiseHistoryCallback(direction, oldline, success).c_str();
    

    return (success) ? strdup(result.c_str()) : nullptr;
}



namespace  {
    //http://stackoverflow.com/questions/7913835/check-if-one-string-is-a-prefix-of-another
    bool prefix(const std::string& a, const std::string& b) {
        if (a.size() > b.size()) {
            return false;
        }
        else {
            return b.substr(0,a.size()) == a;
        }
    }
}


std::string LineNoiseWrapper::LinenoiseHistoryCallback(int direction, std::string oldline,bool &  success)
{
    if (filter == "") {
        
        std::string result = localHistory->Edit(oldline, (direction == 1) ? LineHistory::MoveDirection::prev
                                                : LineHistory::MoveDirection::next,
                                                success);
        
        mstatus = EModificationStatus::history;

        if(!success)
            linenoiseBeep();

        
        return result;

        
    } else {
        
        std::string result,lastReturnedResult;
        bool firstIteration=true;

        //history offset

        int offset=0;

        do {

            result = localHistory->Edit(oldline, (direction == 1) ? LineHistory::MoveDirection::prev
                                                    : LineHistory::MoveDirection::next,
                                                    success);
            offset++;

            if(firstIteration){
                lastReturnedResult = oldline;
                firstIteration =false;
            }



            
            mstatus = EModificationStatus::history;

        } while (success && (!prefix(filter,result)    || (lastReturnedResult == result && prefix(filter,result))));

        if(!success)
        {
            linenoiseBeep();
            //We need to restore previous position
            if(offset)
                offset--; //failed attempt need to balaced
            while (offset) {
                offset--;
                localHistory->Edit(oldline, (direction != 1) ? LineHistory::MoveDirection::prev
                                   : LineHistory::MoveDirection::next,
                                   success);
            }
            success = false;


        }


        return result;
    }
    
    
}

void LineNoiseWrapper::ClearScreen()
{
    linenoiseClearScreen();
}

LineNoiseWrapper::LineNoiseWrapper(LineHistory & linehistory)
:defaultHistory(linehistory)
{
    linenoiseSetHistoryCallback(LineNoiseWrapper::linenoiseHistoryCallbackStatic, this);
    linenoiseSetHintsCallback(LineNoiseWrapper::LinenoiseHistoryHintCallbackStatic, this);
    linenoiseSetFreeHintsCallback(LinenoiseHistoryFreeHintsCallback, this);

}

LineNoiseWrapper::~LineNoiseWrapper()
{
}

