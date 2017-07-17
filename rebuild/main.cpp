//
//  main.cpp
//  rebuild
//  Entry funtion of rebuild. Hosts a runloop.
//
//  Created by Shakthi Prasad G S on 28/01/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "Rebuild.hpp"
#include <iostream>
#include <map>

int main(int argc, const char* argv[])
{
    const std::vector<std::string> arguments(argv, argv + argc); //Doing in c++ way!

    Rebuild rebuild(arguments);

    rebuild.Load();


    while (rebuild.IsAlive()) {
        rebuild.RunStep();
    }

    rebuild.SaveIfLatest();
    return rebuild.GetExitStatus();
}