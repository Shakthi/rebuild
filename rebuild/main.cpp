//
//  main.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 28/01/17.
//  Copyright © 2017 self. All rights reserved.
//

#include <iostream>
#include <map>
#include "Rebuild.hpp"








int main(int argc, const char * argv[]) {
    
    Rebuild rebuild;
    
    
    
    // insert code here...
    std::cout << "Hello, World!\n";
    
    while (rebuild.IsAlive()) {
        rebuild.RunStep();
        
    }
    
    return rebuild.GetExitStatus();
}
