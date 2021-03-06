//
//  StepProcessor.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 02/02/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "StepProcessor.hpp"
#include "Rebuild.hpp"
#include "linenoise/lineNoiseWrapper.hpp"
#include <iostream>

void EchoProcessor::RunStep()
{
    std::string answer = rebuild->lineNoiseWrapper->getLine(Rebuild::prompt);

    if (answer != "") {
        std::cout << "rebuild>" << answer << std::endl;
    } else {
        exitProcessing();
    }
}

void StepProcessor::exitProcessing()
{
    rebuild->exitProcessing();
}
