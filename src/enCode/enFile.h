#ifndef EN_FILE_H
#define EN_FILE_H

#include <string.h>
using namespace std;

enum enFileMode {
  READ,
  WRITE,
  READWRITE
}

class enFile {
 public:
  const char *      GetName() const;
  void              WriteAny( void * data, int len );
  void              WriteChar( const char c );
  void              ReadAny( void * data, int len ) const;
  void              ReadChar( char & c) const;
  enFileMode        _mode;
  size_t            GetMaxSize() const;
  size_t            GetAllocated() const;
 private:
  size_t            _maxsize;
  size_t            _allocated;
};

#endif
