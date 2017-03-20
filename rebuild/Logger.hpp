//
//  Logger.hpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 16/02/17.
//  Copyright © 2017 self. All rights reserved.
//

#ifndef Logger_hpp
#define Logger_hpp
#include <string>

#include <streambuf>
#include <vector>
#include <iostream>
#include <sstream>



class log_buffer : public std::streambuf
{
public:
    explicit log_buffer(std::ostream &sink=std::cout, std::size_t buff_sz = 512);
    bool quit;
protected:
    bool do_caps_and_flush();
    
    std::streamsize sputn( const char_type* s, std::streamsize count )
    {
        return count;
    }

    
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



/*
class Rlog : public std::basic_ostream<char>
{
    int logNestLevel;
    static int currentLogNestLevel;
    static   std::streambuf * sharedbuffer;;
    typedef std::basic_ostream<char> basetype;
    
public:
    
    Rlog();
    ~Rlog();
    Rlog(log_buffer & log);
    
//    template <typename T>
//    Rlog& operator<<( T t) {
//        basetype::operator <<( t);
//         return *this;
//    }
//    
    template <typename T>

    Rlog& operator<<( T&& t) {
        basetype::operator << (std::forward<T>(t));
        return *this;
    }
    
    
    
    
    Rlog& operator<<(basetype & (*__pf)(basetype&))
    {
        return *this;
    }

    
    

};

extern  Rlog rlog; //

*/

struct Rlog:public std::ostream
{
    
    enum class type {
        
        info,
        warn,
        error,
        debug
        
        
    };
    
    

    

    Rlog();
    Rlog(const std::string & incontex);
    Rlog(std::streambuf & buff);
    typedef  bool  (*logfilterType)(int currentlevel,int thislevel,const std::string  & context,int );
    
    
    
    
    
    ~Rlog();
    
    template <typename T>
    friend Rlog& operator<<(Rlog& record, T&& t);
    
    template <typename T>
    void put(T&& t)
    {
        if(Rlog::sharedlogfilter(Rlog::currentLogNestLevel ,logNestLevel,context,currenttype))
        {
            std::ostream & dout=*this;
            dout<< (std::forward<T>(t));
        }
    }
    
    Rlog & operator<<(std::ostream& (*__pf)(std::ostream&))
    {
        
        if(Rlog::sharedlogfilter(Rlog::currentLogNestLevel ,logNestLevel,context,currenttype))
        {
            std::ostream & dout=*this;
            dout<< __pf;;
            
            currenttype = (int)type::info;//FIXME:Temporary fix
        }
        return *this;
    }
    
    
    
    Rlog & operator<<(type atype)
    {
        currenttype=(int)atype;
        
        return *this;
    }
    
    
    
    private:
    
    int logNestLevel;
    int currenttype;
    
    std::string context;
    
    
    
    
    
    static int currentLogNestLevel;
    static std::streambuf * sharedbuffer;
    static logfilterType sharedlogfilter;
    
    static bool defaultFilter(int currentlevel,int thislevel,const std::string & context,int level);
    
    


};


template <typename T>
Rlog& operator<<(Rlog& record, T&& t) {
     record.put( std::forward<T>(t));
    return record;
}




extern Rlog rlog;



#endif /* Logger_hpp */
