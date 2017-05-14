#ifndef E2_I_RENDER_PAYLOAD_HPP
#define E2_I_RENDER_PAYLOAD_HPP

namespace e2 { namespace interface {

enum e_renderpayload_type { //list of primitive resources that can be manipulated
    float_1 = 1,
    float_2,
    float_3,
    float_4,
    float_n,
    mat_3,
    mat_4,
    quat,
    array_float_2,
    array_float_3,
    array_float_n,
    int_1,
    int_2,
    int_3,
    int_4,
    int_n,
    array_int_n,
    string,
};
	
class i_renderpayload {
public:
               size_t _id;
               size_t _buf_id;
               size_t _offset;
               size_t _len;
};
	
} }

#endif
