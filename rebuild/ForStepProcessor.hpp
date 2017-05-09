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
#include "Parser/quickbasic.h"
#include "StatementHistory.hpp"
#include "AST.hpp"



class ForStepProcessor : public BasicStepProcessor {
    ForStatment * thisForBlock;
    
    std::string remarks;
    PopingLineStatementHistory popingLineHistory;
    bool passThrough;
    
public:
    ForStepProcessor(Rebuild* aRebuild,ForStatment * forStatement)
    : BasicStepProcessor(aRebuild),thisForBlock(forStatement)
    {

    }
    
    float  getForVar()
    {
        return varTable[thisForBlock->forVar].getNumVal();
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
    void ExecuteStatments(ForStatment  * forstatement);
    
    
    
};



#endif /* ForStepProcessor_hpp */
