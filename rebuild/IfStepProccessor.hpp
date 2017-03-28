//
//  IfStepProccessor.hpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 12/03/17.
//  Copyright © 2017 self. All rights reserved.
//

#ifndef IfStepProccessor_hpp
#define IfStepProccessor_hpp


#include "BasicStepProccessor.hpp"
#include "quickbasic.h"
#include "StatementHistory.hpp"
#include "AST.hpp"



class IfStepProcessor : public BasicStepProcessor {
    
    std::string remarks;
    IfStatment * ifStatment;//TODO: should be boolean expression
    
    bool passThroughe;
  
    PopingLineStatementHistory popingLineHistory;
    

public:
    IfStepProcessor(Rebuild* aRebuild,IfStatment * aifStatment)
    : BasicStepProcessor(aRebuild),ifStatment(aifStatment)
    {
        // getForVar()=forblock.forBegin->;
       // isInited=false;
    }
    
    
    
    nlohmann::json ToJson()
    {
        nlohmann::json j;
        return j;
    }
    
    void FromJson(nlohmann::json j) {}
    
    void RunStep();
    void Init();
    
    void ExecuteHistory();
    
    
    
};

#endif /* IfStepProccessor_hpp */
