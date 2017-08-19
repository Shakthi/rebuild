//
//  ForStepProcessor.hpp
//  rebuild
//  This processor simulates block for loop in basic language
//  Created by Shakthi Prasad G S on 14/02/17.
//  Copyright © 2017 self. All rights reserved.
//

#ifndef ForStepProcessor_hpp
#define ForStepProcessor_hpp

#include "AST.hpp"
#include "BasicStepProccessor.hpp"
#include "StackedSentenceHistory.hpp"
#include "Value.h"
#include "VarTable.hpp"

class ForStepProcessor : public BasicStepProcessor {
public:
    //How this for block invoked
    enum class InvocationType {
        normal,
        stepin,
        reload
    };

    //Constructor
    ForStepProcessor(Rebuild* aRebuild, ForStatementRef forStatement, VarTable* superTable, InvocationType ainitType = InvocationType::normal);

    //Initialize can be done only after contstrction. After added to history
    void Initialize();

    void RunStep() override;

    void Execute();

private:
    //Stores context data which is valid for one step
    struct ForStepContext : StepContext {
        bool needToRewindHistory;

        ForStepContext()
            : needToRewindHistory(false)
        {
        }
    };

    //Manipulation of  'I' value
    void InitializeI();
    bool EvaluateExitConditionI();
    void DoIncrementI();
    float GetIValue() const;
    Value& GetIVar();

    //Executes the give statement in the context
    void ExecuteAStatement(StatementRef statment);
    //Executes one interation of forloop
    void ExecuteAnIteration();

    //Adds or replace current sentence to history
    void AddToHistory(SentenceRef entry);

    //Detects if perticular statement  can consider as item in listing(I mean programming listing)
    static bool IsListableStatement(SentenceRef s);
    //Move statements from the history to forloop
    void ArchiveStatements();
    //Move statements from the forloop to the history
    void UnArchiveStatements();

    //Run step helpers
    SentenceRef ProcessInput(std::string, LineNoiseWrapper::ExtraResults, StepContext&) override;
    void ProcessSentence(SentenceRef, StepContext&) override;
    void ProcessStatement(StatementRef, StepContext&) override;
    void ProcessCommand(CommandRef, StepContext&) override;
    void UpdateHistory(SentenceRef, StepContext&) override;

    // Data members
    ForStatementRef theStatement;
    InvocationType invocationType;
    StackedSentenceHistory stackedSentenceHistory;
    bool initConditionPassed;
};

#endif /* ForStepProcessor_hpp */