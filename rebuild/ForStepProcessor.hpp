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

    void RunStep();

    void Execute();

    virtual CmdResult Process(CommandRef input);

private:
    //Manipulation of  'I' value
    void InitializeI();
    bool EvaluateExitConditionI();
    void DoIncrementI();
    float GetIValue() const;
    Value & GetIVar();

    //Executes the give statement in the context
    void ExecuteAStatement(StatementRef statment);
    //Executes one interation of forloop
    void ExecuteAnIteration();

    //Adds or replace current sentence to history
    void AddToHistory(SentenceRef entry);

    //Detects if perticular statement  can consider as item in listing(I mean programmiing listing)
    static bool IsListableStatement(SentenceRef s);
    //Move statements from the history to forloop
    void ArchiveStatements();
    //Move statements from the forloop to the history
    void UnArchiveStatements();

    void ProcessSentence(SentenceRef,bool);

    std::string ProcessByMacros(std::string ,LineNoiseWrapper::ExtraResults);
    SentenceRef SentenceFromInput(std::string input,LineNoiseWrapper::ExtraResults);
    SentenceRef Basic_SentenceFromInput(std::string input,LineNoiseWrapper::ExtraResults); //Will be move to super class






    // Data members
    ForStatementRef theStatement;
    InvocationType invocationType;
    StackedSentenceHistory stackedSentenceHistory;
    bool initConditionPassed;
    bool needToRewindHistory;
};

#endif /* ForStepProcessor_hpp */