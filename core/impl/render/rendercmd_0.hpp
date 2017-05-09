#ifndef E2_RENDERCMD_0_HPP
#define E2_RENDERCMD_0_HPP

#include <cstdint>

namespace e2 { namespace render {

    
class rendercmd_0 {
public:
               inline uint64_t extract( ::e2::interface::e_extract_type e ){
		   switch( e ){
		   case ::e2::interface::e_extract_type::cmd_type:
		   {
		       constexpr uint64_t mask_cmd_type = (uint64_t) 0xFF << 56;
		       return ( e & mask_cmd_type ) >> 56;
		   }
		   case ::e2::interface::e_extract_type::render_type:
		   {
		       constexpr uint64_t mask_render_type = (uint64_t) 0xFF << 48;
		       return ( e & mask_render_type ) >> 48;
		   }
		   case ::e2::interface::e_extract_type::id_resource:
		       constexpr uint64_t mask_id_resouce = (uint64_t) 0xFFFFFFFF << 16;
		       return ( e & mask_id_resouce ) >> 16;
		   }
	           case ::e2::interface::e_extract_type::depth:
		       constexpr uint64_t mask_depth = (uint64_t) 0xFF << 8;
		       return ( e & mask_depth ) >> 8;
		   }
	           default:
		       return 0;
	           }
	       }
};

} }

#endif
