//
//  BasicStepProccessor.hpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 04/02/17.
//  Copyright © 2017 self. All rights reserved.
//

#ifndef BasicStepProccessor_hpp
#define BasicStepProccessor_hpp
#include "StepProcessor.hpp"
class BasicStepProcessor:public StepProcessor {
    
    
        
    
    
public:
    
    
    void RunStep();

    
    BasicStepProcessor(Rebuild * aRebuild):StepProcessor(aRebuild)
    {
    }

    
    
};
#endif /* BasicStepProccessor_hpp */
