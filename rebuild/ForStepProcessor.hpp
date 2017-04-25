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
#include "SentenceHistory.hpp"
#include "AST.hpp"



class ForStepProcessor : public BasicStepProcessor {
    ForStatment * thisForBlock;
    
    std::string remarks;
    PopingLineSentenceHistory popingLineHistory;
    bool passThrough;
    
    
public:
    
    enum class InitType {normal,stepin};
    
    ForStepProcessor(Rebuild* aRebuild,ForStatment * forStatement,InitType initType = InitType::normal)
    : BasicStepProcessor(aRebuild),thisForBlock(forStatement)
    {
        
        for (auto st :forStatement->statements ) {
            popingLineHistory.Add(st);
        }

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
    void ExecuteAStep();
    void ExecuteStatments(ForStatment  * forstatement);
    
    virtual bool Process(class Command* input);
    

    
};



#endif /* ForStepProcessor_hpp */
