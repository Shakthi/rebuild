//
//  quickbasic.hpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 11/02/17.
//  Copyright © 2017 self. All rights reserved.
//

#ifndef quickbasic_hpp
#define quickbasic_hpp

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif


#include "quickbasic.tab.h"
#include "location.hh"

namespace yy{
    
    class basicscanner : public yyFlexLexer{
    public:
        
        basicscanner(std::istream *in) : yyFlexLexer(in)
        {
            //loc = new MC::MC_Parser::location_type();
        };
        
        //get rid of override virtual function warning
        using FlexLexer::yylex;
        
        virtual
        yy::parser::token_type yylex( yy::parser::semantic_type * const lval,
                  yy::parser::location_type *location );
        // YY_DECL defined in mc_lexer.l
        // Method body created by flex in mc_lexer.yy.cc
        
        
    private:
        /* yyval ptr */
        yy::parser::semantic_type *yylval = nullptr;
        /* location ptr */
        yy::parser::location_type *loc    = nullptr;
    };
    
} /* end namespace MC */

#endif /* quickbasic_hpp */
