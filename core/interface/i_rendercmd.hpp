#ifndef E2_I_RENDER_CMD_HPP
#define E2_I_RENDER_CMD_HPP

namespace e2 { namespace interface {

enum class e_extract_type {
      cmd_type,
      render_type,
      id_resource,
      depth,
};
    
template<  class DataType, class Impl >
class i_rendercmd {
public:
	       inline DataType extract( e_extract_type e ){ return Impl::extract( e ); }
};

} }
#endif
