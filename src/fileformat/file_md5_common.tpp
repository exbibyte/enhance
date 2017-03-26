#include <fstream>
#include <iostream>
#include <cassert>
#include <string>
#include <utility>
#include <cctype>
#include <cstdio>
#include <unordered_map>
#include <map>
#include <set>

template< typename KeywordContainer >
std::pair< file_md5_common::token, std::string > file_md5_common::get_token( KeywordContainer const & kcontainer, std::fstream & f, bool ignore_comments ){
    if( !file_md5_common::skip_white_space( f ) )
	return std::pair<token,std::string>( token::INVALID, "" );
    int c = f.peek();
    if( EOF == c )
	return std::pair<token,std::string>( token::END, "" );
    std::string s {};
    char sink;
    bool is_signed = false;
    if( '-' == c || '+' == c ){
        is_signed = true;
	s += c;
	f.get(sink);
	c = f.peek();
	if( EOF == c ){
	    return std::pair<token,std::string>( token::INVALID, s );
	}
    }
    if( '.' == c ){
	//float
	s += c;
	f.get(sink);
	c = f.peek();
	while( EOF != c && isdigit(c) )
	{
	    s += c;
	    f.get(sink);
	    c = f.peek();
	}
	return std::pair<token,std::string>( token::FLOAT, s );
    }
    if( isdigit(c) ){
	s += c;
	f.get(sink);
	c = f.peek();
	while( EOF != c && isdigit(c) )
	{
	    s += c;
	    f.get(sink);
	    c = f.peek();
	}
	if( '.' == c ){
	    //float
	    s += '.';
	    f.get(sink);
	    c = f.peek();
	    while( EOF != c && isdigit(c) )
	    {
		s += c;
		f.get(sink);
		c = f.peek();
	    }
	    return std::pair<token,std::string>( token::FLOAT, s );
	}else{
	    //int
	    return std::pair<token,std::string>( token::INT, s );
	}
    }
    if( is_signed ){
	return std::pair<token,std::string>( token::INVALID, s );
    }
    if( '"' == c ){
	f.get(sink);
	c = f.peek();
	while( EOF != c && '"' != c ){
	    s += c;
	    f.get(sink);
	    c = f.peek();
	}
	if( '"' == c ){
	    f.get(sink);
	    return std::pair<token,std::string>( token::STR, s );
	}else{
	    return std::pair<token,std::string>( token::INVALID, s );
	}
    }
    if( '{' == c ){
	f.get(sink);
	return std::pair<token,std::string>( token::BRACEL, "" );
    }
    if( '}' == c ){
	f.get(sink);
	return std::pair<token,std::string>( token::BRACER, "" );
    }
    if( '(' == c ){
	f.get(sink);
	return std::pair<token,std::string>( token::PARENL, "" );
    }
    if( ')' == c ){
	f.get(sink);
	return std::pair<token,std::string>( token::PARENR, "" );
    }
    if( isalpha(c) ){
	s += c;
	f.get(sink);
	c = f.peek();
	while( EOF != c && (isalpha(c) || isdigit(c) || ('_' == c)) ){
	    s += c;
	    f.get(sink);
	    c = f.peek();
	}
	auto it = kcontainer.find( s );
	if( it != kcontainer.end() )
	    return std::pair<token,std::string>( token::KEYWORD, s );
	else
	    return std::pair<token,std::string>( token::STR, s );
    }
    if( '/' == c ){
	s += c;
	f.get(sink);
	c = f.peek();
	if( '/' == c ){
	    s += c;
	    f.get(sink);
	    c = f.peek();
	    while( EOF != c && '\n' != c ){
		s += c;
		f.get(sink);
		c = f.peek();
	    }
	    if( '\n' == c )
		f.get(sink);
	    if( ignore_comments ){
		return get_token( kcontainer, f, ignore_comments );
	    }else{
		return std::pair<token,std::string>( token::COMMENT, s );
	    }
	}
	return std::pair<token,std::string>( token::INVALID, s );
    }
    //skip unrecognized character
    s += c;
    f.get(sink);
    c = f.peek();
    return std::pair<token,std::string>( token::INVALID, s );
}
template< typename KeywordContainer >
bool file_md5_common::aux_process_vec_int( KeywordContainer const & kcontainer, std::fstream & f, int count, void * d, int & retrieved )
{
    //d is assume to be int*
    retrieved = 0;
    int * d_int = ( int * )d;
    for( int i = 0; i < count; ++i ){
	std::pair< token, std::string > t = get_token( kcontainer, f );
	if( token::INT != t.first ){
	    assert( 0 && "expected int" );
	    return false;
	}
	int num;
	int n = sscanf( t.second.c_str(), "%d", &num );
	if( 1 != n ){
	    assert( 0 && "expected int" );
	    return false;
	}
	*(d_int++) = num;
	++retrieved;
    }
    return true;
}
template< typename KeywordContainer >
bool file_md5_common::aux_process_vec_float( KeywordContainer const & kcontainer, std::fstream & f, int count, void * d, int & retrieved )
{
    //d is assume to be float*
    retrieved = 0;
    float * d_float = ( float * )d;
    for( int i = 0; i < count; ++i ){
	std::pair< token, std::string > t = get_token( kcontainer, f );
	if( token::FLOAT != t.first && token::INT != t.first ){
	    assert( 0 && "expected float or int" );
	    return false;	    
	}
	float num;
	int n = sscanf( t.second.c_str(), "%f", &num );
	if( 1 != n ){
	    assert( 0 && "expected float or int" );
	    return false;
	}
	*(d_float++) = num;
	++retrieved;
    }
    return true;
}
template< typename KeywordContainer >
bool file_md5_common::aux_process_int( KeywordContainer const & kcontainer, std::fstream & f, void * d )
{
    //d is assumed to be int*
    std::pair< token, std::string > t = get_token( kcontainer, f );
    if( token::INT != t.first ){
	return false;
    }else{
	int num;
	int n = sscanf( t.second.c_str(), "%d", &num );
	if( 1 != n ){
	    return false;   
	}
	*((int*)d) = num;
	return true;
    }
}
template< typename KeywordContainer >
bool file_md5_common::aux_process_float( KeywordContainer const & kcontainer, std::fstream & f, void * d )
{
    //d is assumed to be float*
    std::pair< token, std::string > t = get_token( kcontainer, f );
    if( token::FLOAT != t.first && token::INT != t.first ){
	return false;	    
    }
    float num;
    int n = sscanf( t.second.c_str(), "%f", &num );
    if( 1 != n ){
	return false;
    }
    *((float*)d)= num;
    return true;
}
