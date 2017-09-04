//
//  ReadStepProcessor.hpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 12/05/17.
//  Copyright © 2017 self. All rights reserved.
//

#ifndef ReadStepProcessor_hpp
#define ReadStepProcessor_hpp

#include "StepProcessor.hpp"
#include "VarTable.hpp"
#include <list>


class ReadStepProcessor : public StepProcessor {
    std::list<std::string> list;
    std::string prompt;
    VarTable & varTable;



public:
    ReadStepProcessor(Rebuild* aRebuild, std::list<std::string>& alist,std::string aPrompt, VarTable & aVartable);
    nlohmann::json ToJson();
    void FromJson(nlohmann::json j);
    std::string  GetPrompt(){ return "input]";}
    std::string SetPrompt(std::string aprompt);

    void RunStep();
};

#endif /* ReadStepProcessor_hpp */
