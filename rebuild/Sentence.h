//
//  Sentence.h
//  rebuild
//
//  Created by Shakthi Prasad G S on 08/07/17.
//  Copyright © 2017 self. All rights reserved.
//

#ifndef Sentence_h
#define Sentence_h
#include "nlohmann/json.hpp"
#include "cereal/cereal.hpp"


struct Sentence
{
    std::string sourceText; //Debug only
    virtual ~Sentence(){}
    virtual Sentence  * clone ()const =0;




    nlohmann::json ToJson();
    static  Sentence * GetFromJson(nlohmann::json );


    //TODO make visitor pattern
    template<class Archive>
    void serialize( Archive & ar )
    { ar( CEREAL_NVP(sourceText) ); }


    virtual bool isEqual(const Sentence & that){ return this -> dumpToString()  == that.dumpToString();}
    virtual std::string dumpToString()const {return sourceText;}



};


typedef std::shared_ptr<Sentence>   SentenceRef;

struct Statement:Sentence
{

    virtual Statement  * clone ()const = 0;

};

typedef std::shared_ptr<Statement>   StatementRef;

struct Command:Sentence
{
    virtual Command  * clone ()const = 0;

    
};



#endif /* Sentence_h */
