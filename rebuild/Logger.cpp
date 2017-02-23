//
//  Logger.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 16/02/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "Logger.hpp"
#include "Rebuild.hpp"
#include <iostream>
#include <cassert>

log_buffer::int_type log_buffer::overflow(int_type ch)
{
    if (sink_ && ch != traits_type::eof())
    {
        assert(std::less_equal<char *>()(pptr(), epptr()));
        *pptr() = ch;
        pbump(1);
        if (do_caps_and_flush())
            return ch;
    }
    
    return traits_type::eof();
}
int log_buffer::sync()
{
    return do_caps_and_flush() ? 0 : -1;
}

log_buffer::log_buffer(std::ostream &sink, std::size_t buff_sz ):sink_(sink),buffer_(buff_sz),quit(false)
{
    char *base = &buffer_.front();
    setp(base, base + buffer_.size() - 1); // -1 to make overflow() easier

}




bool log_buffer::do_caps_and_flush()
{
    std::ptrdiff_t n = pptr() - pbase();
    pbump(static_cast<int>(-n));
    std::string prompt=Rebuild::prompt+"  ///";
    
    sink_.write(prompt.c_str(),prompt.size());
    return sink_.write(pbase(), n).good();
}




Rlog::Rlog(std::streambuf & lbuffer):std::ostream(&lbuffer)
{
    Rlog::sharedbuffer = &lbuffer;
}


Rlog::Rlog():std::ostream(Rlog::sharedbuffer)
{
    Rlog::currentLogNestLevel++;
    logNestLevel = Rlog::currentLogNestLevel;
}

Rlog::~Rlog()
{
    Rlog::currentLogNestLevel--;
    
}




int Rlog::currentLogNestLevel = 0;
std::streambuf * Rlog::sharedbuffer = nullptr;




Rlog& operator<<(Rlog& record,std::ostream & (*__pf)(std::ostream&))
{
    return record;;
}