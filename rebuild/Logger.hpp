//
//  Logger.hpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 16/02/17.
//  Copyright © 2017 self. All rights reserved.
//

#ifndef Logger_hpp
#define Logger_hpp

#include <streambuf>
#include <vector>
#include <iostream>


class log_buffer : public std::streambuf
{
public:
    explicit log_buffer(std::ostream &sink=std::cout, std::size_t buff_sz = 512);
    bool quit;
protected:
    bool do_caps_and_flush();
    
private:
    int_type overflow(int_type ch);
    int sync();
    
    // copy ctor and assignment not implemented;
    // copying not allowed
    log_buffer(const log_buffer &);
    log_buffer &operator= (const log_buffer &);
    
private:
    bool cap_next_;
    std::ostream &sink_;
    std::vector<char> buffer_;
};


extern std::basic_ostream<char>  rlog; //



#endif /* Logger_hpp */
