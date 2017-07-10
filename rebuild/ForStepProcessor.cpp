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

void ForStepProcessor::ExecuteIncrement(std::shared_ptr<ForStatment> statement)
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

        auto statement = std::dynamic_pointer_cast<Statement>(*i);
        if (statement) {

            auto errorStatemnt = std::dynamic_pointer_cast<ErrorStatement>(statement);
            if (!errorStatemnt)
                thisForBlock->statements.push_back(StatementRef( statement->clone()));
        }
    }
}

void ForStepProcessor::RunStep()
{

    if (initConditionPassed == true) {

        LineNoiseWrapper::ExtraResults extraResults;

        std::string prefilled = "";
        bool proceedStroke = false;

        if(!statementStash.empty()){

            const StatementRef statment =  statementStash.back();
            prefilled = statment->dumpToString();

        }





        std::string answer = rebuild->lineNoiseWrapper->getLineWithHistory(
            Rebuild::prompt + "for " + thisForBlock->forVar + "]:",
            popingLineHistory,extraResults,prefilled);
        

        SentenceRef result = nullptr;

        if (answer == "") {
            if (extraResults.status == LineNoiseWrapper::ExitStatus::ctrl_c) {
                exitProcessing();
                return;
            }


            if (extraResults.status == LineNoiseWrapper::ExitStatus::ctrl_X) {
                proceedStroke = true;
                answer = ProcessCtrlKeyStroke(extraResults.ctrlKey);
            }
        }

        if (extraResults.status == LineNoiseWrapper::ExitStatus::ok
            && extraResults.mstatus == LineNoiseWrapper::EModificationStatus::ok) {

            if (!statementStash.empty()) {
                result =  statementStash.back();
                statementStash.pop_back();
            }


        }else {

            BasicParser parser;
            result = parser.Parse(answer);
            if (!statementStash.empty()) {
                statementStash.pop_back();
            }


        }



        auto nextStatemnt = std::dynamic_pointer_cast<NextStatement>(result);
        if (nextStatemnt) {
            ExecuteAStep();
            popingLineHistory.Add(result);

            return;
        }

        auto endStatement = std::dynamic_pointer_cast<EndStatement>(result);
        if (endStatement) {

            ArchiveStatements();
            for (ExecuteIncrement() ;CheckCondition(); ExecuteIncrement()) {
                for (auto i = popingLineHistory.rcbegin(); i != popingLineHistory.rcend();
                     i++) {

                    ExecuteTheStatment(std::dynamic_pointer_cast<Statement>((*i)));
                }
            }
            exitProcessing();
            return;
        }

        auto errorStatemnt = std::dynamic_pointer_cast<ErrorStatement>(result);
        if (errorStatemnt) {
            BasicStepProcessor::Evaluate(errorStatemnt);
            popingLineHistory.Add(errorStatemnt);
            return;
        }

        {
            auto statemnt = std::dynamic_pointer_cast<Statement>(result);
            CmdResult res = BasicStepProcessor::Evaluate(statemnt);
            if (statemnt && res.handled) {
                if (res.addtoHistory)
                    popingLineHistory.Add(result);
            } else {
                res = Process(std::dynamic_pointer_cast<Command>(result));
                if (res.addtoHistory)
                    popingLineHistory.Add(result);
            }
        }
            } else {
        LineNoiseWrapper::ExtraResults extraResults;
        std::string answer = rebuild->lineNoiseWrapper->getLineWithHistory(
            "[rebuild>forelse]:", popingLineHistory,extraResults);

        BasicParser parser;
        SentenceRef result = parser.Parse(answer);

        auto endStatement = std::dynamic_pointer_cast<EndStatement>(result);
        if (endStatement) {
            ArchiveStatements();
            //Exucte pending

            for (ExecuteIncrement() ;CheckCondition(); ExecuteIncrement()) {
                for (auto i = popingLineHistory.rcbegin(); i != popingLineHistory.rcend();
                     i++) {

                    ExecuteTheStatment(std::dynamic_pointer_cast<Statement>((*i)));
                }
            }


            exitProcessing();
            return;
        }

        auto errorStatemnt = std::dynamic_pointer_cast<ErrorStatement>(result);
        if (errorStatemnt) {
            BasicStepProcessor::Evaluate(errorStatemnt);
            popingLineHistory.Add(errorStatemnt);
            return;
        }

        auto statemnt = std::dynamic_pointer_cast<Statement>(result);

        if (BasicStepProcessor::Evaluate(statemnt).addtoHistory )
            popingLineHistory.Add(result);
    }
}

void ForStepProcessor::ExecuteStatments(std::shared_ptr<ForStatment> thisForBlock)
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

BasicStepProcessor::CmdResult ForStepProcessor::Process(std::shared_ptr<Command> input)
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

            for (auto i = popingLineHistory.begin(); i != popingLineHistory.end();
                 i++) {

                auto statmentCasted = std::dynamic_pointer_cast<Statement>((*i));
                if (statmentCasted) {

                    statementStash.push_back(statmentCasted);
                    popingLineHistory.Splice(i,false);

                    break;
                }
            }

            return positiveResult;

        } else if (customCommand->name == "popback") {

            for (auto i = popingLineHistory.begin(); i != popingLineHistory.end();
                 i++) {
                if (std::dynamic_pointer_cast<Statement>((*i))) {
                    popingLineHistory.Splice(i);
                    break;
                }
            }

            return positiveResult;

        } else if (customCommand->name == "runall") { // This command  makes next iteration of loop

            for (Init(); CheckCondition(); ExecuteIncrement()) {
                for (auto i = popingLineHistory.rcbegin(); i != popingLineHistory.rcend();
                     i++) {

                    ExecuteTheStatment(std::dynamic_pointer_cast<Statement>((*i)));
                }
            }

            return positiveResult;

        } else if (customCommand->name == "run") { // One loop

            for (Init(); CheckCondition();) {
                for (auto i = popingLineHistory.rcbegin(); i != popingLineHistory.rcend();
                     i++) {

                    ExecuteTheStatment(std::dynamic_pointer_cast<Statement>((*i)));
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
void ForStepProcessor::ExecuteTheStatment(std::shared_ptr<Statement> st)
{
    if (std::dynamic_pointer_cast<ErrorStatement>(st))
        return;

    Evaluate(st);
}

void ForStepProcessor::ExecuteAStep()
{

    ExecuteIncrement();
    for (; CheckCondition();) {

        for (auto i = popingLineHistory.rcbegin(); i != popingLineHistory.rcend();
             i++) {

            ExecuteTheStatment(std::dynamic_pointer_cast<Statement>((*i)));
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

            Evaluate(std::dynamic_pointer_cast<Statement>((*i)));
        }
    }
}