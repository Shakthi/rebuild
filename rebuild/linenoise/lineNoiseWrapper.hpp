//
//  lineNoiseWrapper.hpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 31/01/17.
//  Copyright © 2017 self. All rights reserved.
//

#ifndef lineNoiseWrapper_hpp
#define lineNoiseWrapper_hpp

#include <string>
#include "lineHistory.hpp"


class LineNoiseWrapper :public Rebuild::Serialised{
    
    LineHistory history;
    
public:
    enum class EStatus
    {
        ok,
        ctrl_c,
        ctrl_d
    } ;
private:
    EStatus status;
public:
    
    
    
    std::string getLine(std::string prompt);
    EStatus  GetStatus(){ return status;}
    ~LineNoiseWrapper();
    LineNoiseWrapper();
    static  const char * linenoiseHistoryCallback(int direction, const char * oldline,void * context);
    
    
    
    nlohmann::json ToJson();
    void FromJson(nlohmann::json);
    

};
#endif /* lineNoiseWrapper_hpp */
