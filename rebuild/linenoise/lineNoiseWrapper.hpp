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


class LineNoiseWrapper {
    
    LineHistory history;
    
public:
    
    std::string getLine(std::string prompt);
    ~LineNoiseWrapper();
    LineNoiseWrapper();
    static  const char * linenoiseHistoryCallback(int direction, const char * oldline,void * context);
};
#endif /* lineNoiseWrapper_hpp */
