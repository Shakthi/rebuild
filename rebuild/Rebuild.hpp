//
//  Rebuild.hpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 31/01/17.
//  Copyright © 2017 self. All rights reserved.
//

#ifndef Rebuild_hpp
#define Rebuild_hpp

#include <stdio.h>

class Rebuild {
    
    
    
    bool alive;
    enum  ExitStatus
    {
        exitStatusRIP=0,
        exitStatusLowMemory=3
        
    } exitStatus;
    
    
    
    
    
    class LineNoiseWrapper * lineNoiseWrapper;
    
    
    
public:
    
    bool IsAlive() { return alive;}
    ExitStatus GetExitStatus() { return exitStatus;}
    
    Rebuild();
    ~Rebuild();
    void Save();
    void Load();
    
    
    void RunStep();
    
    
};


#endif /* Rebuild_hpp */


