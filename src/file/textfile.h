// textfile.h: interface for reading and writing text files
// www.lighthouse3d.com
//
// You may use these functions freely.
// they are provided as is, and no warranties, either implicit,
// or explicit are given
//////////////////////////////////////////////////////////////////////

#ifndef TEXTFILE_H
#define TEXTFILE_H

#include <cstdlib>
#include <cstdio>
#include <cstring>

char *textFileRead(char const * fn);
int textFileWrite(char const * fn, char *s);

#endif
