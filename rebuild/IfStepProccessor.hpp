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
#include "lineHistory.hpp"
#include "AST.hpp"



class IfStepProcessor : public BasicStepProcessor {
    
    std::string remarks;
    RelationalExpression * expresion;//TODO: should be boolean expression
    
    bool passThroughe;
  
    PopingLineHistory popingLineHistory;
    

public:
    IfStepProcessor(Rebuild* aRebuild,RelationalExpression * nexpresion)
    : BasicStepProcessor(aRebuild),expresion(nexpresion)
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
    
    void ExecuteLoop();
    
    
    
};

#endif /* IfStepProccessor_hpp */
