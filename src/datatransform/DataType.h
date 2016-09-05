#ifndef DATA_TYPE_H
#define DATA_TYPE_H

class DataQuery {
public:
    enum Enum {
	INPUT,
	OUTPUT,
    };
};

class DataType {
public:
    enum Enum {
	FILE_POLYMESH = 0,
	FILE_ANIMATION,
	FILE_USER_PROFILE,
	FILE_MACHINE_PROFILE,
	FILE_OTHER,
	DATASTRUCT_POLYMESH,
	DATASTRUCT_POLYMESH_ARRAY,
	ENUM_SIZE,
    };
};

#endif
