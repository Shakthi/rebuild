//
//  BasicStepProccessor.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 04/02/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "BasicStepProccessor.hpp"
#include "ForStepProcessor.hpp"
#include "IfStepProccessor.hpp"
#include "Rebuild.hpp"

#include "Logger.hpp"
#include "Parser/ParserWrapper.hpp"
#include "Sentence.h"
#include "SentenceHistory.hpp"
#include "Value.h"
#include "linenoise/lineNoiseWrapper.hpp"

#include "ReadStepProcessor.hpp"
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

SentenceRef BasicStepProcessor::ProcessInput(std::string input, LineNoiseWrapper::ExtraResults extraResults, StepContext& aStepContext)
{
    SentenceRef result = nullptr;

    if (extraResults.status == LineNoiseWrapper::ExitStatus::ctrl_c) {

        exitProcessing();

    } else if (extraResults.status == LineNoiseWrapper::ExitStatus::ok && extraResults.mstatus == LineNoiseWrapper::EModificationStatus::history && !aStepContext.macroSubstituted) {

        auto lastiter = history->GetLastStatmentIter();

        assert(*lastiter);
        result = SentenceRef((*lastiter)->clone());

    } else if (extraResults.status == LineNoiseWrapper::ExitStatus::ok) {

        BasicParser parser;
        result = parser.Parse(input);
    }

    return result;
}

void BasicStepProcessor::UpdateHistory(SentenceRef result, StepContext& stepContext)
{

    if (result && stepContext.addtoHistory) {
        history->Add(result);
    }
}

void BasicStepProcessor::ProcessSentence(SentenceRef result, StepContext& aStepContext)
{

    StepContext stepContext;
    auto statement = std::dynamic_pointer_cast<Statement>(result);
    if (statement) {

        ProcessStatement(statement, stepContext);

    } else {

        auto command = std::dynamic_pointer_cast<Command>(result);
        ProcessCommand(command, stepContext);
    }

    UpdateHistory(result, stepContext);
}


void BasicStepProcessor::ExecuteStatement(StatementRef st){


    StepContext cmd;
    ProcessStatement(st,cmd);

    rebuild->Substep(this);

}

void BasicStepProcessor::ProcessStatement(StatementRef result, StepContext& stepContext)
{
    stepContext.handled = stepContext.addtoHistory = true;

    if (result == nullptr) {
        stepContext.addtoHistory = false;
        stepContext.handled = false;
        return;
    }

    auto endStatement = std::dynamic_pointer_cast<EndStatement>(result);
    if (endStatement) {
        exitProcessing();
        stepContext.addtoHistory = false;
        stepContext.handled = true;
        return;
    }

    auto readStatemnt = std::dynamic_pointer_cast<ReadStatement>(result);
    if (readStatemnt) {

        auto readStepProcessor = std::shared_ptr<StepProcessor>(new ReadStepProcessor(rebuild, readStatemnt->variableList, readStatemnt->prompt, localVarTable));
        rebuild->AddNewProcessing(readStepProcessor);
        return; //TODO:need to add to history later
    }

    auto forStatemnt = std::dynamic_pointer_cast<ForStatement>(result);
    if (forStatemnt) {

        if (!forStatemnt->statements.empty()) {

            auto processor = new ForStepProcessor(rebuild, forStatemnt,
                &localVarTable, ForStepProcessor::InvocationType::reload);
            processor->Initialize();

            rebuild->AddNewProcessing(std::shared_ptr<StepProcessor>(processor));
            processor->Execute();
            processor->exitProcessing();

        } else {

            auto processor = new ForStepProcessor(rebuild, forStatemnt, &localVarTable);
            processor->Initialize();

            rebuild->AddNewProcessing(std::shared_ptr<StepProcessor>(processor));
        }

        return; //TODO:need to add to history later
    }

    auto errorStatemnt = std::dynamic_pointer_cast<ErrorStatement>(result);
    if (errorStatemnt) {

        std::cout << Rebuild::GetPrompt() << "! " << errorStatemnt->description << std::endl;
        return; //TODO and as temporary
    }

    auto ifstment = std::dynamic_pointer_cast<IfStatment>(result);
    if (ifstment) {
        auto* stepprocessor = new IfStepProcessor(rebuild, ifstment, &localVarTable);

        stepprocessor->Init();

        rebuild->AddNewProcessing(std::shared_ptr<StepProcessor>(stepprocessor));
        return; //TODO:need to add to history later
    }

    auto printStatemnt = std::dynamic_pointer_cast<PrintStatement>(result);
    if (printStatemnt) {

        for (auto i = printStatemnt->printitems.begin(); i != printStatemnt->printitems.end(); i++) {

            Value value = (*i)->Evaluate(&localVarTable);
            if (value.valutype == Value::Evaluetype::stringtype)
                std::cout << value.getStringVal();
            else if (value.valutype == Value::Evaluetype::floattype)
                std::cout << value.getNumVal();
            else if (value.valutype == Value::Evaluetype::emptyType)
                std::cout << "<NULL>";
            else if (value.valutype == Value::Evaluetype::booltype)
                std::cout << (value.getBoolVal() ? "true" : "false");
        }
        if (!std::dynamic_pointer_cast<PrintElementStatement>(result))
            std::cout << std::endl;

        return;
    }

    auto letStatemnt = std::dynamic_pointer_cast<LetStatement>(result);
    if (letStatemnt) {
        if (localVarTable.GetVar(letStatemnt->variablename).valutype == Value::Evaluetype::emptyType)
            localVarTable.GetVar(letStatemnt->variablename) = letStatemnt->rvalue->Evaluate(&localVarTable);
        else
            std::cout << Rebuild::GetPrompt() << "! " << letStatemnt->variablename << " already defined" << std::endl;

        return;
    }

}

void BasicStepProcessor::ProcessCommand(std::shared_ptr<Command> result, StepContext& stepContext)
{

    if (result == nullptr) {
        stepContext.addtoHistory = false;
        stepContext.handled = false;
        return;
    }



    auto statemnt = std::dynamic_pointer_cast<ListCommand>(result);
    if (statemnt) {

        std::cout << std::endl;
        int count = 1;
        for (auto i = history->rcbegin(); i != history->rcend(); i++) {
            std::cout << count << " " << (*i)->dumpToString() << std::endl;
            count++;
        }

        return;
    }
    auto customCommand = std::dynamic_pointer_cast<CustomCommand>(result);
    if (customCommand) {

        if (customCommand->name == "restart") {
            rebuild->Restart();

        } else if (customCommand->name == "cls") {

            rebuild->lineNoiseWrapper.ClearScreen();
            return;

        } else if (customCommand->name == "stepin") {

            if (history->GetLastStatmentIter() != history->end()) {

                SentenceRef sentence = *(history->GetLastStatmentIter());
                auto forStatement = std::dynamic_pointer_cast<ForStatement>(sentence);
                if (forStatement) {

                    auto forStepProcessor = new ForStepProcessor(rebuild, forStatement, &localVarTable, ForStepProcessor::InvocationType::stepin);
                    rebuild->AddNewProcessing(std::shared_ptr<StepProcessor>(forStepProcessor));
                    forStepProcessor->Initialize();
                }
            }

            return;

        } else {
            rlog << Rlog::type::error << "Not found command" << customCommand->name << std::endl;
            return; //Need to add temporarray
        }
    }


    stepContext.addtoHistory = false;
    stepContext.handled = false;
}

std::string BasicStepProcessor::ProcessCtrlKeyStroke(int ctrlchar)
{
    Rlog rlog;
    switch (ctrlchar + 'a' - 1) {
    case 'z':
        return ".restart";
        break;
    case 'x':
        return ".stepin";
        break;

    case 's':
        return ".checkback";
        break;

    default:
        rlog << "\n"
             << std::string("key") + char('a' + ctrlchar - 1) << std::endl;
        return std::string("key") + char('a' + ctrlchar - 1);
        break;
    }
}

std::string BasicStepProcessor::ProcessByMacros(std::string input, LineNoiseWrapper::ExtraResults& result, StepContext& stepContext)
{
    Rlog rlog;
    if (result.status == LineNoiseWrapper::ExitStatus::ctrl_X) {
        
        stepContext.macroSubstituted = true;
        result.status = LineNoiseWrapper::ExitStatus::ok;
        char ctrlchar = result.ctrlKey;
        return ProcessCtrlKeyStroke(ctrlchar);
    }

    return input;
}

void BasicStepProcessor::ProcessStep(std::string answer, LineNoiseWrapper::ExtraResults& extraResults, StepContext& stepContext)
{

    answer = ProcessByMacros(answer, extraResults, stepContext);
    SentenceRef result = ProcessInput(answer, extraResults, stepContext);
    ProcessSentence(result, stepContext);

}

void BasicStepProcessor::RunStep()
{
    LineNoiseWrapper::ExtraResults extraResults;
    std::string answer = rebuild->lineNoiseWrapper.getLineWithHistory(Rebuild::GetPrompt() + ":", rebuild->history, extraResults);
    StepContext stepContext;

    ProcessStep(answer, extraResults, stepContext);
}

nlohmann::json BasicStepProcessor::ToJson()
{
    nlohmann::json j;
    j["localvarablelist"] = localVarTable.ToJson();

    return j;
}
void BasicStepProcessor::FromJson(nlohmann::json j)
{
    localVarTable.FromJson(j["localvarablelist"]);
}