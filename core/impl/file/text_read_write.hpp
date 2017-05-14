#ifndef E2_TEXT_READ_WRITE_HPP
#define E2_TEXT_READ_WRITE_HPP

#include <vector>

namespace e2 { namespace file {

class text_read_write {
public:
    static bool read(char const * fn, std::vector<char> * read );
    static bool write(char const * fn, char const * s );
};

} }

#endif
