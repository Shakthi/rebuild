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


class LineNoiseWrapper {
    
    
public:
    
    std::string getLine(std::string prompt);
    ~LineNoiseWrapper();
    LineNoiseWrapper();
    
};
#endif /* lineNoiseWrapper_hpp */
