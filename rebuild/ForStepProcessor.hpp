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
#include "lineHistory.hpp"
#include "AST.hpp"



class ForStepProcessor : public BasicStepProcessor {
    const ForStatment::ForBlock thisForBlock;
    
    std::string remarks;
    PopingLineHistoryStr popingLineHistory;
    bool passThrough;
    
public:
    ForStepProcessor(Rebuild* aRebuild,  ForStatment::ForBlock && forblock)
    : BasicStepProcessor(aRebuild)
    ,thisForBlock(std::move(forblock))
    {
       // getForVar()=forblock.forBegin->;

    }
    
    float  getForVar()
    {
        return varTable[thisForBlock.forVar].getNumVal();
    }
    
    
    nlohmann::json ToJson()
    {
        nlohmann::json j;
        return j;
    }
    
    void FromJson(nlohmann::json j) {}
    
    void RunStep();
    void Init();
    
    void ExecuteLoop();
    
    
    
};



#endif /* ForStepProcessor_hpp */
