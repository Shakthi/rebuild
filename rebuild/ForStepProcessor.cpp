//
//  ForStepProcessor.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 14/02/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "ForStepProcessor.hpp"
#include "Parser/ParserWrapper.hpp"
#include "linenoise/lineNoiseWrapper.hpp"

bool ForStepProcessor::CheckCondition()
{

    return (getForVar() <= thisForBlock->forEnd->Evaluate(&localVarTable).getNumVal());
}

void ForStepProcessor::Init()
{

    localVarTable.GetVar(thisForBlock->forVar) = thisForBlock->forBegin->Evaluate(&localVarTable);

    if (CheckCondition())
        initConditionPassed = true;
    else
        initConditionPassed = false;
}

void ForStepProcessor::ExecuteIncrement(ForStatment* statement)
{
    if (statement != nullptr)
        thisForBlock = statement;

    localVarTable.GetVar(thisForBlock->forVar) = localVarTable.GetValue(thisForBlock->forVar).getNumVal() + thisForBlock->forStep->Evaluate(&localVarTable).getNumVal();
}

void ForStepProcessor::ArchiveStatements()
{
    thisForBlock->statements.clear();
    for (auto i = popingLineHistory.rcbegin(); i != popingLineHistory.rcend();
         i++) {

        auto statement = dynamic_cast<Statement*>(*i);
        if (statement) {

            auto errorStatemnt = dynamic_cast<ErrorStatement*>(statement);
            if (!errorStatemnt)
                thisForBlock->statements.push_back(statement->clone());
        }
    }
}

void ForStepProcessor::RunStep()
{

    if (initConditionPassed == true) {

        LineNoiseWrapper::ExtraResults extraResults;

        std::string answer = rebuild->lineNoiseWrapper->getLineWithHistory(
            Rebuild::prompt + "for " + thisForBlock->forVar + "]:",
            popingLineHistory,extraResults);

        BasicParser parser;
        Sentence* result = parser.Parse(answer);

        if (answer == "") {
            if (extraResults.status == LineNoiseWrapper::ExitStatus::ctrl_c) {
                exitProcessing();
                return;
            }
        }

        auto nextStatemnt = dynamic_cast<NextStatement*>(result);
        if (nextStatemnt) {
            ExecuteAStep();
            popingLineHistory.Add(result);

            return;
        }

        auto endStatement = dynamic_cast<EndStatement*>(result);
        if (endStatement) {
            delete result;

            ArchiveStatements();
            for (ExecuteIncrement() ;CheckCondition(); ExecuteIncrement()) {
                for (auto i = popingLineHistory.rcbegin(); i != popingLineHistory.rcend();
                     i++) {

                    ExecuteTheStatment(dynamic_cast<Statement*>((*i)));
                }
            }
            exitProcessing();
            return;
        }

        auto errorStatemnt = dynamic_cast<ErrorStatement*>(result);
        if (errorStatemnt) {
            BasicStepProcessor::Evaluate(errorStatemnt);
            popingLineHistory.Add(errorStatemnt);
            return;
        }

        {
            auto statemnt = dynamic_cast<Statement*>(result);
            CmdResult res = BasicStepProcessor::Evaluate(statemnt);
            if (statemnt && res.handled) {
                if (res.addtoHistory)
                    popingLineHistory.Add(result);
            } else {
                res = Process(dynamic_cast<Command*>(result));
                if (res.addtoHistory)
                    popingLineHistory.Add(result);
            }
        }
            } else {
        LineNoiseWrapper::ExtraResults extraResults;
        std::string answer = rebuild->lineNoiseWrapper->getLineWithHistory(
            "[rebuild>forelse]:", popingLineHistory,extraResults);

        BasicParser parser;
        Sentence* result = parser.Parse(answer);

        auto endStatement = dynamic_cast<EndStatement*>(result);
        if (endStatement) {
            delete result;
            ArchiveStatements();
            //Exucte pending

            for (ExecuteIncrement() ;CheckCondition(); ExecuteIncrement()) {
                for (auto i = popingLineHistory.rcbegin(); i != popingLineHistory.rcend();
                     i++) {

                    ExecuteTheStatment(dynamic_cast<Statement*>((*i)));
                }
            }


            exitProcessing();
            return;
        }

        auto errorStatemnt = dynamic_cast<ErrorStatement*>(result);
        if (errorStatemnt) {
            BasicStepProcessor::Evaluate(errorStatemnt);
            popingLineHistory.Add(errorStatemnt);
            return;
        }

        auto statemnt = dynamic_cast<Statement*>(result);

        if (BasicStepProcessor::Evaluate(statemnt).addtoHistory )
            popingLineHistory.Add(result);
    }
}

void ForStepProcessor::ExecuteStatments(ForStatment* thisForBlock)
{
    // localVarTable.SetSuper(&outer);

    for (localVarTable.GetVar(thisForBlock->forVar) = localVarTable.GetVar(thisForBlock->forVar).getNumVal();
         localVarTable.GetValue(thisForBlock->forVar).getNumVal() <= thisForBlock->forEnd->Evaluate(&localVarTable).getNumVal();
         ExecuteIncrement(thisForBlock)) {

        for (auto i = thisForBlock->statements.begin();
             i != thisForBlock->statements.end(); i++) {

            Evaluate((*i));
        }
    }
}

BasicStepProcessor::CmdResult ForStepProcessor::Process(Command* input)
{
    CmdResult positiveResult{ true, true };

    auto statemnt = dynamic_cast<ListCommand*>(input);
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

    auto customCommand = dynamic_cast<CustomCommand*>(input);
    if (customCommand) {

        if (customCommand->name == "checkback") {

            for (auto i = popingLineHistory.begin(); i != popingLineHistory.end();
                 i++) {
                if (dynamic_cast<Statement*>((*i))) {
                    popingLineHistory.Splice(i);
                    break;
                }
            }

            return positiveResult;

        } else if (customCommand->name == "popback") {

            for (auto i = popingLineHistory.begin(); i != popingLineHistory.end();
                 i++) {
                if (dynamic_cast<Statement*>((*i))) {
                    popingLineHistory.Splice(i);
                    break;
                }
            }

            return positiveResult;

        } else if (customCommand->name == "runall") { // This command  makes next iteration of loop

            for (Init(); CheckCondition(); ExecuteIncrement()) {
                for (auto i = popingLineHistory.rcbegin(); i != popingLineHistory.rcend();
                     i++) {

                    ExecuteTheStatment(dynamic_cast<Statement*>((*i)));
                }
            }

            return positiveResult;

        } else if (customCommand->name == "run") { // One loop

            for (Init(); CheckCondition();) {
                for (auto i = popingLineHistory.rcbegin(); i != popingLineHistory.rcend();
                     i++) {

                    ExecuteTheStatment(dynamic_cast<Statement*>((*i)));
                }

                break;
            }

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

// excute filterring error
void ForStepProcessor::ExecuteTheStatment(Statement* st)
{
    if (dynamic_cast<ErrorStatement*>(st))
        return;

    Evaluate(st);
}

void ForStepProcessor::ExecuteAStep()
{

    ExecuteIncrement();
    for (; CheckCondition();) {

        for (auto i = popingLineHistory.rcbegin(); i != popingLineHistory.rcend();
             i++) {

            ExecuteTheStatment(dynamic_cast<Statement*>((*i)));
        }

        break;
    }
}

void ForStepProcessor::ExecuteHistory()
{

    for (localVarTable.GetVar(thisForBlock->forVar) = getForVar() + thisForBlock->forStep->Evaluate(&localVarTable).getNumVal();
         getForVar() <= thisForBlock->forEnd->Evaluate(&localVarTable).getNumVal();
         localVarTable.GetVar(thisForBlock->forVar) = getForVar() + thisForBlock->forStep->Evaluate(&localVarTable).getNumVal()) {

        for (auto i = popingLineHistory.rcbegin(); i != popingLineHistory.rcend();
             i++) {

            Evaluate(dynamic_cast<Statement*>((*i)));
        }
    }
}