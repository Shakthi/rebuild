//
//  BasicStepProccessor.hpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 04/02/17.
//  Copyright © 2017 self. All rights reserved.
//

#ifndef BasicStepProccessor_hpp
#define BasicStepProccessor_hpp
#include "Rebuild.hpp"

#include "StepProcessor.hpp"

class BasicStepProcessor : public StepProcessor {
public:
    void RunStep();
    virtual bool Evaluate(class Statement* input);

    BasicStepProcessor(Rebuild* aRebuild)
        : StepProcessor(aRebuild)
    {
    }

    nlohmann::json ToJson();
    void FromJson(nlohmann::json);
};
#endif /* BasicStepProccessor_hpp */
