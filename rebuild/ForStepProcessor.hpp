//
//  ForStepProcessor.hpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 14/02/17.
//  Copyright © 2017 self. All rights reserved.
//

#ifndef ForStepProcessor_hpp
#define ForStepProcessor_hpp

#include "BasicStepProccessor.hpp"
#include "quickbasic.h"



class ForStepProcessor : public BasicStepProcessor {
    const ForStatment::ForBlock thisForBlock;
    float forValue;
    
    std::vector<std::string> statements;
    
public:
    ForStepProcessor(Rebuild* aRebuild, const ForStatment::ForBlock & forblock)
    : BasicStepProcessor(aRebuild)
    ,thisForBlock(forblock)
    {
        forValue=forblock.forBegin;
    }
    
    nlohmann::json ToJson()
    {
        nlohmann::json j;
        return j;
    }
    
    void FromJson(nlohmann::json j) {}
    
    void RunStep();
    
    void ExecuteLoop();
    
    
    
};



#endif /* ForStepProcessor_hpp */
