#ifndef FILE_MD5_COMMON_HPP
#define FILE_MD5_COMMON_HPP

#include <string>
#include <unordered_set>
#include <list>
#include <vector>
#include <iostream>
#include <map>
#include <fstream>

class file_md5_common {
public:
    enum class token {
	COMMENT,
	KEYWORD,
	STR,
	INT,
	FLOAT,
	BRACEL,
	BRACER,
	PARENL,
	PARENR,
	INVALID,
	END,
    };
    static bool skip_white_space( std::fstream & f ){
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
    template< typename KeywordContainer >
    static std::pair<token, std::string> get_token( KeywordContainer const & kcontainer, std::fstream & f, bool ignore_comments = true );
    template< typename KeywordContainer >
    static bool aux_process_vec_int( KeywordContainer const & kcontainer, std::fstream &, int count, void *, int & retrieved );
    template< typename KeywordContainer >
    static bool aux_process_vec_float( KeywordContainer const & kcontainer, std::fstream &, int count, void *, int & retrieved );
    template< typename KeywordContainer >
    static bool aux_process_int( KeywordContainer const & kcontainer, std::fstream &, void * );
    template< typename KeywordContainer >
    static bool aux_process_float( KeywordContainer const & kcontainer, std::fstream &, void * );
};

#include "file_md5_common.tpp"

#endif
