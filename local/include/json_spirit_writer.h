#ifndef JSON_SPIRIT_WRITER
#define JSON_SPIRIT_WRITER

//          Copyright John W. Wilkinson 2007 - 2011
// Distributed under the MIT License, see accompanying file LICENSE.txt

// json spirit version 4.04

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include "json_spirit_writer_template.h"
#include <iostream>

namespace json_spirit
{
    // these functions to convert JSON Values to text

    void         write( const Value&  value, std::ostream&  os, unsigned int options = 0 );
    void         write( const mValue& value, std::ostream&  os, unsigned int options = 0 );
    std::string  write( const Value&  value, unsigned int options = 0 );
    std::string  write( const mValue& value, unsigned int options = 0 );

#ifndef BOOST_NO_STD_WSTRING
    void         write( const wValue&  value, std::wostream& os, unsigned int options = 0 );
    void         write( const wmValue& value, std::wostream& os, unsigned int options = 0 );
    std::wstring write( const wValue&  value, unsigned int options = 0 );
    std::wstring write( const wmValue& value, unsigned int options = 0 );
#endif

    // these "formatted" versions of the "write" functions are the equivalent of the above functions
    // with option "pretty_print"
    
    void         write_formatted( const Value& value, std::ostream&  os );
    void         write_formatted( const mValue& value, std::ostream&  os );
    std::string  write_formatted( const Value& value );
    std::string  write_formatted( const mValue& value );

#ifndef BOOST_NO_STD_WSTRING
    void         write_formatted( const wValue& value, std::wostream& os );
    void         write_formatted( const wmValue& value, std::wostream& os );
    std::wstring write_formatted( const wValue& value );
    std::wstring write_formatted( const wmValue& value );
#endif
}

#endif
