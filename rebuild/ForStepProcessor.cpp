//
//  ForStepProcessor.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 14/02/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "ForStepProcessor.hpp"
#include "Logger.hpp"
#include "Parser/ParserWrapper.hpp"
#include "linenoise/lineNoiseWrapper.hpp"

ForStepProcessor::ForStepProcessor(Rebuild* aRebuild, ForStatementRef forStatement, VarTable* superTable, InvocationType ainitType)
    : BasicStepProcessor(aRebuild, superTable)
    , theStatement(forStatement)
    , stackedSentenceHistory(aRebuild->GetHistoryStack())
    , needToRewindHistory(false)
    , invocationType(ainitType)
{
    history = &stackedSentenceHistory;
}

float ForStepProcessor::GetIValue() const
{
    return localVarTable.GetValue(theStatement->var).getNumVal();
}


Value & ForStepProcessor::GetIVar()
{
    return localVarTable.GetVar(theStatement->var);
}


bool ForStepProcessor::IsListableStatement(SentenceRef s)
{
    auto st = std::dynamic_pointer_cast<Statement>(s);
    if (!st) {
        return false;
    }

    if (std::dynamic_pointer_cast<NextStatement>(st)) {
        return false;
    }
    if (std::dynamic_pointer_cast<ErrorStatement>(st)) {
        return false;
    }

    if (std::dynamic_pointer_cast<UnProcessedStatment>(st)) {
        return false;
    }

    return true;
}

bool ForStepProcessor::EvaluateExitConditionI()
{
    const auto endvalue = theStatement->forEnd->Evaluate(&localVarTable).getNumVal();
    const auto forValue = GetIValue();

    return (forValue <= endvalue);
}

void ForStepProcessor::Initialize()
{

    UnArchiveStatements();
    if (invocationType == InvocationType::stepin) {

        stackedSentenceHistory.Rewind();
    }
    InitializeI();
}

void ForStepProcessor::InitializeI()
{
    const auto beginvalue = theStatement->forBegin->Evaluate(&localVarTable).getNumVal();
    GetIVar() = beginvalue;

    if (EvaluateExitConditionI())
        initConditionPassed = true;
    else
        initConditionPassed = false;
}

void ForStepProcessor::DoIncrementI()
{

    const auto forValue = GetIValue();

    GetIVar() = forValue + theStatement->forStep->Evaluate(&localVarTable).getNumVal();

}

void ForStepProcessor::UnArchiveStatements()
{
    for (auto st : theStatement->statements) {
        stackedSentenceHistory.Add(st);
    }

}

void ForStepProcessor::ArchiveStatements()
{
    theStatement->statements.clear();
    for (auto i = stackedSentenceHistory.rcbegin(); i != stackedSentenceHistory.rcend();
         i++) {

        auto statement = std::dynamic_pointer_cast<Statement>(*i);
        if (statement) {

            auto errorStatemnt = std::dynamic_pointer_cast<ErrorStatement>(statement);
            if (!errorStatemnt)
                theStatement->statements.push_back(StatementRef(statement->clone()));
        }
    }
}

void ForStepProcessor::AddToHistory(SentenceRef entry)
{

    auto lastiter = stackedSentenceHistory.GetHistoryWritePointer();
    if (std::dynamic_pointer_cast<UnProcessedStatment>(*lastiter)) {
        stackedSentenceHistory.Add(entry);
    } else {
        stackedSentenceHistory.Replace(entry);
    }
}

void ForStepProcessor::ProcessSentence(SentenceRef result,bool isMacroExampanded)
{
    auto nextStatemnt = std::dynamic_pointer_cast<NextStatement>(result);
    if (nextStatemnt) {
        ExecuteAnIteration();
        AddToHistory(result);

        return;
    }

    auto endStatement = std::dynamic_pointer_cast<EndStatement>(result);
    if (endStatement) {

        ArchiveStatements();
        for (DoIncrementI(); EvaluateExitConditionI(); DoIncrementI()) {
            for (auto i = stackedSentenceHistory.rcbegin(); i != stackedSentenceHistory.rcend();
                 i++) {

                ExecuteAStatement(std::dynamic_pointer_cast<Statement>((*i)));
            }
        }
        exitProcessing();
        return;
    }

    auto errorStatemnt = std::dynamic_pointer_cast<ErrorStatement>(result);
    if (errorStatemnt) {
        BasicStepProcessor::Evaluate(errorStatemnt);
        AddToHistory(errorStatemnt);
        return;
    }

    {
        auto statemnt = std::dynamic_pointer_cast<Statement>(result);
        CmdResult res = BasicStepProcessor::Evaluate(statemnt);
        if (statemnt && res.handled) {
            if (res.addtoHistory)
                AddToHistory(result);
        } else {
            res = Process(std::dynamic_pointer_cast<Command>(result));
            if (res.addtoHistory && !isMacroExampanded)
                AddToHistory(result);
            if (needToRewindHistory) {
                stackedSentenceHistory.Rewind();
                needToRewindHistory = false;
            }
        }
    }

}

SentenceRef ForStepProcessor::SentenceFromInput(std::string input,LineNoiseWrapper::ExtraResults extraResults)
{
    SentenceRef result = nullptr;


    if(extraResults.status == LineNoiseWrapper::ExitStatus::ok  && extraResults.mstatus == LineNoiseWrapper::EModificationStatus::ok) {

        result = *(stackedSentenceHistory.GetHistoryWritePointer());
        assert(result);


    }else{

        result = Basic_SentenceFromInput(input, extraResults);


    }


    return result;


}


SentenceRef ForStepProcessor::Basic_SentenceFromInput(std::string input,LineNoiseWrapper::ExtraResults extraResults)
{
    SentenceRef result = nullptr;

    if (extraResults.status == LineNoiseWrapper::ExitStatus::ctrl_c) {

        exitProcessing();

    }else if (extraResults.status == LineNoiseWrapper::ExitStatus::ok && extraResults.mstatus == LineNoiseWrapper::EModificationStatus::history) {

        auto lastiter = history->GetLastStatmentIter();

        assert(*lastiter);
        result = SentenceRef((*lastiter)->clone());

    }else if(extraResults.status == LineNoiseWrapper::ExitStatus::ok ) {

        BasicParser parser;
        result = parser.Parse(input);
    }




    return result;
}



void ForStepProcessor::RunStep()
{
    //For statement will execute atleast once
    if (initConditionPassed == true) {

        LineNoiseWrapper::ExtraResults extraResults;

        std::string prompt = Rebuild::GetPrompt() + "for " + theStatement->var + "]:";
        std::string answer = rebuild->lineNoiseWrapper.getLineWithHistory(prompt, stackedSentenceHistory, extraResults);

        bool isInputProcesseedByMacro = false;
        answer = BasicStepProcessor::ProcessByMacros(answer, extraResults, isInputProcesseedByMacro);
        

        SentenceRef result = SentenceFromInput(answer, extraResults);

        ProcessSentence(result, isInputProcesseedByMacro);

    } else {
        Rlog rlog;

        rlog << "To be implemented";
    }
}


void ForStepProcessor::Execute()
{
    for (InitializeI(); EvaluateExitConditionI(); DoIncrementI()) {
        for (auto i = stackedSentenceHistory.rcbegin(); i != stackedSentenceHistory.rcend();
             i++) {

            ExecuteAStatement(std::dynamic_pointer_cast<Statement>((*i)));
        }
    }
}

BasicStepProcessor::CmdResult ForStepProcessor::Process(CommandRef input)
{
    CmdResult positiveResult{ true, true };

    auto statemnt = std::dynamic_pointer_cast<ListCommand>(input);
    if (statemnt) {

        std::cout << std::endl;
        int count = 1, tabstop = 0;

        auto historyStackIter = rebuild->GetHistoryStack().begin();
        auto historyStackIterEnd = rebuild->GetHistoryStack().end();

        while (historyStackIter != historyStackIterEnd) {

            for (auto i = (*historyStackIter)->rcbegin();
                 i != (*historyStackIter)->rcend(); i++) {

                if (ForStepProcessor::IsListableStatement(*i)) {
                    std::cout << count;

                    for (int i = 0; i < tabstop + 1; i++)
                        std::cout << " ";
                    std::cout << (*i)->dumpToString() << std::endl;
                    count++;
                }
            }
            historyStackIter++;
            tabstop++;
        }

        return positiveResult;
    }

    auto customCommand = std::dynamic_pointer_cast<CustomCommand>(input);
    if (customCommand) {

        if (customCommand->name == "checkback") {

            for (auto i = stackedSentenceHistory.begin(); i != stackedSentenceHistory.end();
                 i++) {

                auto statmentCasted = std::dynamic_pointer_cast<Statement>((*i));
                if (statmentCasted) {

                    //                    statementStash.push_back(statmentCasted);
                    stackedSentenceHistory.PopBack();

                    break;
                }
            }
            positiveResult.addtoHistory = false;
            return positiveResult;

        } else if (customCommand->name == "runall") { // This command  makes next iteration of loop

            for (InitializeI(); EvaluateExitConditionI(); DoIncrementI()) {
                for (auto i = stackedSentenceHistory.rcbegin(); i != stackedSentenceHistory.rcend();
                     i++) {

                    ExecuteAStatement(std::dynamic_pointer_cast<Statement>((*i)));
                }
            }

            return positiveResult;

        } else if (customCommand->name == "run") { // One loop

            for (InitializeI(); EvaluateExitConditionI();) {
                for (auto i = stackedSentenceHistory.rcbegin(); i != stackedSentenceHistory.rcend();
                     i++) {

                    ExecuteAStatement(std::dynamic_pointer_cast<Statement>((*i)));
                }

                break;
            }

            return positiveResult;

        } else if (customCommand->name == "rewind") { // One loop

            needToRewindHistory = true;
            return positiveResult;
        } else if (customCommand->name == "list") {

            std::cout << std::endl;
            int count = 1, tabstop = 0;

            auto historyStackIter = rebuild->GetHistoryStack().begin();
            auto historyStackIterEnd = rebuild->GetHistoryStack().end();

            while (historyStackIter != historyStackIterEnd) {

                for (auto i = (*historyStackIter)->rcbegin();
                     i != (*historyStackIter)->rcend(); i++) {

                    std::cout << count;

                    for (int i = 0; i < tabstop + 1; i++)
                        std::cout << " ";
                    std::cout << (*i)->dumpToString() << std::endl;
                    count++;
                }
                historyStackIter++;
                tabstop++;
            }

        } else {
            return BasicStepProcessor::Process(input);
        }
    }

    return CmdResult{ false, false };
}

// execute filtering error
void ForStepProcessor::ExecuteAStatement(StatementRef st)
{
    if (std::dynamic_pointer_cast<ErrorStatement>(st))
        return;

    Evaluate(st);
}

void ForStepProcessor::ExecuteAnIteration()
{

    DoIncrementI();
    for (; EvaluateExitConditionI();) {

        for (auto i = stackedSentenceHistory.rcbegin(); i != stackedSentenceHistory.rcend();
             i++) {

            ExecuteAStatement(std::dynamic_pointer_cast<Statement>((*i)));
        }

        break;
    }
}