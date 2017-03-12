#include "file_md5_mesh.hpp"

#include <fstream>
#include <iostream>
#include <cassert>
#include <string>
#include <utility>
#include <cctype>
#include <cstdio>
#include <map>

std::map<std::string, void(*)(typename file_md5_mesh::token, std::fstream &) > file_md5_mesh::_keyword_map = \
{ { "MD5Version", 0 }, { "commandline", 0 }, { "numJoints", 0 }, { "numMeshes", 0 }, \
  { "joints", 0 }, { "mesh", 0 }, { "shader", 0 }, { "numverts", 0 }, { "vert", 0 }, \
  { "numtris", 0 }, { "tri", 0 }, { "numweights", 0 }, { "weight", 0 } };
    
file_md5_mesh::data_wrap file_md5_mesh::process( std::string file_path ){
    std::fstream f( file_path, std::fstream::in );
    std::pair<token, std::string> t = get_token( f );
    while( token::END != t.first ){
	// if( token::INVALID == t.first ){
	//     assert( 0 && "invalid token encountered" );
	//     break;
	// }
	process_token( t, f );
	t = get_token( f );
    }
    return {};
}

bool file_md5_mesh::skip_white_space( std::fstream & f ){
    while( f.good() ){
	int c = f.peek();
	if( EOF == c )
	    return true;
	else if( ' ' == c || '\n' == c || '\t' == c || '\r' == c ){
	    int a = f.get();
	    continue;
	}
	else
	    return true;
    }
    return false;
}


std::pair< file_md5_mesh::token, std::string > file_md5_mesh::get_token( std::fstream & f ){
    if( !skip_white_space( f ) )
	return std::pair<token,std::string>( token::INVALID, "" );
    int c = f.peek();
    if( EOF == c )
	return std::pair<token,std::string>( token::END, "" );
    std::string s {};
    char sink;
    bool negative = false;
    if( '-' == c ){
	negative = true;
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
    if( negative ){
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
	auto it = _keyword_map.find( s );
	if( it != _keyword_map.end() )
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
	    return std::pair<token,std::string>( token::COMMENT, s );
	}
	return std::pair<token,std::string>( token::INVALID, s );
    }
    //skip unrecognized character
    s += c;
    f.get(sink);
    c = f.peek();
    return std::pair<token,std::string>( token::INVALID, s );
}

void file_md5_mesh::process_token( std::pair<file_md5_mesh::token, std::string> t, std::fstream & f ){
    if( token::KEYWORD == t.first ){
	std::cout << "found keyword: " << t.second << std::endl;
    }
    if( token::INT == t.first ){
	std::cout << "found int: " << t.second << std::endl;
    }
    if( token::FLOAT == t.first ){
	std::cout << "found float: " << t.second << std::endl;
    }
    if( token::STR == t.first ){
	std::cout << "found str: " << t.second << std::endl;
    }
    if( token::COMMENT == t.first ){
	std::cout << "found comment: " << t.second << std::endl;
    }
    if( token::BRACEL == t.first ){
	std::cout << "found {" << std::endl;
    }
    if( token::BRACER == t.first ){
	std::cout << "found }" << std::endl;
    }
    if( token::PARENL == t.first ){
	std::cout << "found (" << std::endl;
    }
    if( token::PARENR == t.first ){
	std::cout << "found )" << std::endl;
    }
    if( token::INVALID == t.first ){
	std::cout << "found invalid: " << t.second << std::endl;
    }
}

// bool file_md5_mesh::get_number( std::fstream & f ){
//     string s {};
//     while( f.good() ){
// 	int c = f.peek();
// 	if( EOF == c )
// 	    return false;
// 	else if( ' ' == c || '\n' == c || '\t' == c || '\r' == c ){
// 	    int a = f.get();
// 	    continue;
// 	}
// 	else
// 	    return true;
//     }
// }
