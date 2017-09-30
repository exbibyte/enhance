#ifndef E2_I_LP_HPP
#define E2_I_LP_HPP

#include <cstring>
#include <vector>


namespace e2 { namespace states { namespace lp {
    enum class e_lp_ans { //type of answer
	NONE,
	BOUNDED,
	UNBOUNDED,
    };
} } }
    
namespace e2 { namespace interface {
	
template< class T >
class i_lp {
public:
    static bool init( T & s, std::vector< std::vector< double > > a, std::vector< double > b, std::vector< double > c ){
	return false;
    }
    static std::pair< ::e2::states::lp::e_lp_ans, std::vector<double> /*values of input variables*/ > solve( T & s ){
	return { ::e2::states::lp::e_lp_ans::NONE, {} };
    }
};

} }

#endif


