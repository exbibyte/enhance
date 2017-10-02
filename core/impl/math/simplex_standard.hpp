#ifndef SIMPLEX_STANDARD_HPP
#define SIMPLEX_STANDARD_HPP

#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdio>
#include <cassert>
#include <utility>
#include <tuple>
#include <set>
#include <limits>
#include <map>

#include "i_lp.hpp"

namespace e2 { namespace math {

class simplex_standard {
public:
    const double _m_error_delta = 1e-9; //error threshold used for evaluating auxialliary objective function
    int _m_original; //length of the original variables
    int _n; //number of rows
    int _m; //number of columns, including added slack variables
    std::vector< std::vector< double > > _a; //constraint matrix coefficients
    std::vector< double > _b; //constraint right hand side
    std::vector< double > _c; //objective function coefficients
    double _d; //objective function constant
    double _d_aux;
    double _d_normalization; //keep track of the scaling factor for objective function
    std::set< int > _basic; //variables to be substituted into other non-basic variable equations and has contributes to the objective function value
    std::set< int > _non_basic; //variables that are candidates when selecting the next variable to enter based on marginal gain in objectiv function value
    int _num_aux; //number of auxilliary variables
    std::vector< double > _c_aux; //auxilliary objective function
    
    std::map<int, int> _map_basic_to_row; //keeps track of which row the basic variable is in

    std::map<int, int> _map_row_to_basic; //keeps track of which basic variable is in which row
    
    ///helper functions
    static double dot( std::vector<double> & coeff, std::vector<double> & vals ){
	double z = 0;
	assert( coeff.size() == vals.size() );
	for( size_t i = 0; i < coeff.size(); ++i ){
	    z += coeff[i] * vals[i];
	}
	return z;
    }
    
    static bool init( simplex_standard & s, std::vector< std::vector< double > > a, std::vector< double > b, std::vector< double > c ){
	///sanity checks the input and saves the input to be manipulated 
	if( a.size() == 0 ){
	    return false;
	}
	size_t z = a[0].size();
	for( size_t i = 0; i < a.size(); ++i ){
	    if( z != a[i].size() ){
		return false;
	    }
	}
	if( b.size() != a.size() ){
	    return false;
	}
	if( z != c.size() ){
	    return false;
	}
	s._n = a.size();
	s._m = z; //slack to be added later
	s._m_original = z;
	s._num_aux = 0;
	s._a = a;
	s._b = b;
	s._c = c;
	s._d = 0;
	s._d_aux = 0;
	s._d_normalization = 1.0;
	return true;
    }
    // static double compute_objective_value( simplex_standard & s ){
    // 	double d = 0;
    // 	//only basic variables contribute to the current value of the objective function
    // 	//this assumes the basic variables are normalized to unity already
    // 	for( auto & i : s._basic ){
    // 	    int row = s._map_basic_to_row[i];
    // 	    d += s._c[i] * s._b[row];
    // 	}
    // 	return d;
    // }
	
    static std::pair<int, std::pair<int,int> > compute_next_pivot( simplex_standard & s ){
	int pivot_col = -1;
	int pivot_row = -1;
	// cout << "# pivot" << endl;

	// print( s );

	//use devex/steepest approximation rule for pivot selection
	bool possible = false;
	double max_increase = std::numeric_limits<double>::min();
	size_t row_max_increase_overall = -1;
	size_t col_max_increase_overall = -1;
	bool has_unbounded_overall = false;
	bool has_no_solution_overall = false;
	for( auto i : s._non_basic ){
	    size_t row_max_increase = -1;
	    size_t col_max_increase = -1;
	    bool has_unbounded = false;
	    bool has_no_solution = false;
	    if( s._c[i] > 0 ){
		double max_increase_for_col = std::numeric_limits<double>::min();
		for( size_t j = 0; j< s._n; ++j ){ //for each row
		    if( ( s._a[j][i] < 0 && s._b[j] <= 0 ) ||
			( s._a[j][i] > 0 && s._b[j] >= 0 ) )
		    {
			double increase = s._b[j]/s._a[j][i] * s._c[i];
			if( max_increase_for_col == std::numeric_limits<double>::min() ){
			    max_increase_for_col = increase;
			    row_max_increase = j;
			}else{
			    if( increase < max_increase_for_col ){
				max_increase_for_col = increase;
				row_max_increase = j;
			    }
			}
		    }else if( s._a[j][i] == 0 ){
			has_unbounded = true;
			// cout << "here 1" << endl;
		    }else if( s._a[j][i] < 0 && s._b[i] >= 0 ){ // negative coeff on left, positive on right
			has_unbounded = true;
			// cout << "here 2" << endl;
		    }else if( s._a[j][i] >= 0 && s._b[i] < 0 ){ // positive coeff on left, negative on right
			has_no_solution = true;
			// cout << "here 2" << endl;
		    }
		}
		if( max_increase_for_col != std::numeric_limits<double>::min() ){
		    if( max_increase_for_col > max_increase ){
			max_increase = max_increase_for_col;
			col_max_increase = i;

			row_max_increase_overall = row_max_increase;
			col_max_increase_overall = col_max_increase;
			possible = true;
		    }
		}else {
		    if( has_no_solution ){
			has_no_solution_overall = true;
		    }else{
			has_unbounded_overall = true;
		    }
		}
	    }
	}
	if( possible ){
	    pivot_col = col_max_increase_overall;
	    return { 2, { row_max_increase_overall, col_max_increase_overall } }; //possible optmization
	}
	if( has_no_solution_overall ){
	    return { 1, {-1,-1} }; //no solution
	}
	if( has_unbounded_overall ){
	    return { 3, {-1,-1} }; //unbounded	    
	}
	return { 0, {-1,-1} }; //optimal
    }

    static std::pair<int, std::pair<int,int> > compute_next_pivot_aux( simplex_standard & s ){

	// cout << "# pivot aux" << endl;
	// print_aux( s );

	if( s._d_aux == 0 ){
	    return { 0, {-1,-1} }; //aux var met
	}		
	bool aux_var_done = true;
	int count_aux_var_not_done = 0;
	for( size_t i = s._m - s._num_aux; i < s._m; ++i ){
	    if( s._basic.find( i ) != s._basic.end() ){
		// cout << "aux var " << i << " not done" << endl;
		aux_var_done = false;
		++count_aux_var_not_done;
		break;
	    }
	}
	if( aux_var_done ){
	    // cout << "aux var done" << endl;
	    return { 0, {-1,-1} }; //aux var met
	}else{
	    // cout << count_aux_var_not_done << endl;
	}

	int pivot_col = -1;
	int pivot_row = -1;

	//use devex/steepest approximation rule for pivot selection
	bool possible = false;
	double max_increase = std::numeric_limits<double>::min();
	size_t row_max_increase_overall = -1;
	size_t col_max_increase_overall = -1;
	bool has_unbounded_overall = false;
	bool has_no_solution_overall = false;
	for( auto i : s._non_basic ){
	    size_t row_max_increase = -1;
	    size_t col_max_increase = -1;
	    bool has_unbounded = false;
	    bool has_no_solution = false;
	    if( s._c_aux[i] > 0 ){
		double max_increase_for_col = std::numeric_limits<double>::min();
		for( size_t j = 0; j< s._n; ++j ){ //for each row
		    if( ( s._a[j][i] < 0 && s._b[j] <= 0 ) ||
			( s._a[j][i] > 0 && s._b[j] >= 0 ) )
		    {
			double increase = s._b[j]/s._a[j][i] * s._c_aux[i];
			if( max_increase_for_col == std::numeric_limits<double>::min() ){
			    max_increase_for_col = increase;
			    row_max_increase = j;
			}else{
			    if( increase < max_increase_for_col ){
				max_increase_for_col = increase;
				row_max_increase = j;
			    }
			}
		    }else if( s._a[j][i] == 0 ){
			has_unbounded = true;
			// cout << "here 1" << endl;
		    }else if( s._a[j][i] < 0 && s._b[i] >= 0 ){ // negative coeff on left, positive on right
			has_unbounded = true;
			// cout << "here 2" << endl;
		    }else if( s._a[j][i] >= 0 && s._b[i] < 0 ){ // positive coeff on left, negative on right
			has_no_solution = true;
			// cout << "here 2" << endl;
		    }
		}
		if( max_increase_for_col != std::numeric_limits<double>::min() ){
		    if( max_increase_for_col > max_increase ){
			max_increase = max_increase_for_col;
			col_max_increase = i;

			row_max_increase_overall = row_max_increase;
			col_max_increase_overall = col_max_increase;
			possible = true;
		    }
		}else {
		    if( has_no_solution ){
			has_no_solution_overall = true;
		    }else{
			has_unbounded_overall = true;
		    }
		}
	    }
	}

	if( possible ){
	    pivot_col = col_max_increase_overall;
	    return { 2, { row_max_increase_overall, col_max_increase_overall } }; //possible optmization
	}
	if( has_no_solution_overall ){
	    return { 1, {-1,-1} }; //no solution
	}
	if( has_unbounded_overall ){
	    return { 3, {-1,-1} }; //unbounded	    
	}
	// cout << "d_aux: " << s._d_aux << endl;
	if( s._d_aux < s._m_error_delta ){
	    return { 0, {-1,-1} }; //should be optimal, but some aux var is still a basic variable	   
	}
	return { 1, {-1,-1} }; //no solution
    }

    static bool gaussian_elimination( simplex_standard & s, std::pair<int,int> pivot ){
	// cout << "# eliminate" << endl;
	
	int row = pivot.first;
	int col = pivot.second;
	//normalizing constant
	double k = 1.0/s._a[row][col];
	//normalize the pivot row and the right hand side constraint
	for( size_t i = 0; i < s._m; ++i ){
	    s._a[row][i] *= k;
	}
	s._b[row] *= k;
	
	//eliminate variable in other rows
	for( size_t i = 0; i < s._n; ++i ){
	    if( i == row ){
		continue;
	    }
	    double d = s._a[i][col];
	    if( d != 0 ){
		//normalize row by substracting a multiple of the pivot row from the current row
		for( size_t j = 0; j < s._m; ++j ){
		    double pivot_val = d * s._a[row][j];
		    s._a[i][j] -= pivot_val;
		}
		s._b[i] -= d * s._b[row];
	    }
	}
	//eliminate variable in objective function as well
	{
	    double d = s._c[col];
	    for( size_t i = 0; i < s._m; ++i ){
 		s._c[i] -= d * s._a[row][i];
	    }
	    s._d -= d * s._b[row];
	}
	//update basic and nonbasic sets
	s._basic.insert( col );
	s._non_basic.erase( col );

	int old_col = s._map_row_to_basic[ row ];
	s._map_row_to_basic[ row ] = col;

	s._map_basic_to_row.erase( old_col );
	s._map_basic_to_row[col] = row;
		
	s._non_basic.insert( old_col );
	s._basic.erase( old_col );

	// cout << "entering col: " << col << ", exiting col: " << old_col << endl;

	// for( auto i : s._map_row_to_basic ){
	//     cout << "row: " << i.first << ", basic var: " << i.second << endl;
	// }

	return true;
    }

    static bool gaussian_elimination_aux( simplex_standard & s, std::pair<int,int> pivot ){
	// cout << "# eliminate aux" << endl;
	// print_aux( s );

	int row = pivot.first;
	int col = pivot.second;
	//normalizing constant
	double k = 1.0/s._a[row][col];
	//normalize the pivot row and the right hand side constraint
	for( size_t i = 0; i < s._m; ++i ){
	    s._a[row][i] *= k;
	}
	s._b[row] *= k;
	
	//eliminate variable in other rows
	for( size_t i = 0; i < s._n; ++i ){
	    if( i == row ){
		continue;
	    }
	    double d = s._a[i][col]; //value in the same column as the pivot variable
	    double scale;
	    if( d != 0 ){
		scale = d;
		//normalize row by substracting a multiple of the pivot row from the current row
		for( size_t j = 0; j < s._m; ++j ){
		    double pivot_row_val = s._a[row][j];
		    s._a[i][j] -= scale * pivot_row_val;
		}
		s._b[i] -= scale * s._b[row];
	    }
	}
	//eliminate variable in objective function as well
	{
	    double d = s._c_aux[col];
	    double scale;
	    if( d != 0 ){
		scale = d;
		for( size_t i = 0; i < s._m; ++i ){
		    double pivot_row_val = s._a[row][i];
		    s._c_aux[i] -= scale * pivot_row_val;
		}
		s._d_aux -= scale * s._b[row];
	    }
	}
	//update basic and nonbasic sets
	s._basic.insert( col );
	s._non_basic.erase( col );

	int old_col = s._map_row_to_basic[ row ];
	s._map_row_to_basic[ row ] = col;

	s._non_basic.insert( old_col );
	s._basic.erase( old_col );

	s._map_basic_to_row.erase( old_col );
	s._map_basic_to_row[col] = row;
	
	// cout << "entering col: " << col << ", exiting col: " << old_col << endl;

	// for( auto i : s._map_row_to_basic ){
	//     cout << "row: " << i.first << ", basic var: " << i.second << endl;
	// }
	
	// cout << "# post eliminate aux" << endl;
	// print_aux( s );
	
	return true;
    }
    
    static std::pair< ::e2::states::lp::e_lp_ans, std::vector<double> > solve( simplex_standard & s ){
	//add slack variables to coefficient matrix and init basic and non-basic variables
	size_t col = 0;
	for( size_t i = 0; i < s._m; ++i ){
	    s._non_basic.insert( col );
	    ++col;
	}
	for( size_t i = 0; i < s._n; ++i ){
	    //add slack variables
	    for( size_t j = 0; j < i; ++j ){
		s._a[i].push_back( 0.0 );
	    }
	    s._a[i].push_back( 1.0 );
	    for( size_t j = i+1; j < s._n; ++j ){
		s._a[i].push_back( 0.0 );
	    }	    

	    //set slack variables as basic variables and set objective function coefficients for slack variables to be zero
	    s._basic.insert( col );
	    s._c.push_back( 0.0 );
	    ++col;

	    s._map_row_to_basic[i] = s._m + i;
	}

	s._m += s._n;

	//introduce artificial variable if necessary for phase 1 to get a basic feasible solution
	std::vector<int> row_aux; //which rows to add the auxilliary variables
	for( size_t i = 0; i < s._n; ++i ){
	    if( s._b[i] < 0 ){
	        row_aux.push_back(i);
		++s._num_aux;
	    }
	}
	//pad zeros
	for( size_t i = 0; i < s._n; ++i ){
	    for( size_t j = 0; j < row_aux.size(); ++j ){
		s._a[i].push_back( 0.0 );
	    }
	}
	//aux objective function
	if( s._num_aux > 0 ){
	    s._c_aux.resize( s._m + s._num_aux, 0.0 );
	}
	//set values for aux vars
	std::set<int> col_aux;
	for( size_t j = 0; j < row_aux.size(); ++j ){
	    int r = row_aux[j];
	    int c = s._m; //the column id for the aux variable
	    ++s._m;
	    s._a[r][c] = -1.0;
	    int old_basic = s._map_row_to_basic[r];
	    s._basic.erase( old_basic );
	    s._non_basic.insert( old_basic );
	    s._basic.insert( c ); //set new basic to the aux variable col
	    s._c_aux[c] = -1.0; //set aux obj function coeff
	    col_aux.insert( c );
	    s._map_row_to_basic[r] = c;
	}

	// int index_r = 0;
	// for( auto & i : s._a ){
	//     for( auto & j : i ){
	// 	cout << j << " ";
	//     }
	//     cout << " " << s._b[index_r++] << endl;
	// }

	// cout << "----" << endl;

	// cout << "non-basic: ";
	// for( auto & i : s._non_basic ){
	//     cout << i << " ";
	// }
	// cout << endl;

	// cout << "basic: ";
	// for( auto & i : s._basic ){
	//     cout << i << " ";
	// }
	// cout << endl;

	// cout << "phase 1" << endl;


	// cout << "aux init" << endl;

	// print_aux( s );
		
	//phase 1
	//eliminate auxilliary variables from aux objective function
	// for( auto & col: s._basic ){
	// double d = s._c_aux[col];
	if( s._num_aux > 0 ){
	    for( size_t i = 0; i < s._n; ++i ){
		auto it_find = s._map_row_to_basic.find( i );
		if( it_find != s._map_row_to_basic.end() ){
		    if( col_aux.find( it_find->second ) != col_aux.end() ){
			int r = i;
			int c = it_find->second;
			// cout << "row: " << i << ", basic var: " << c << endl;
			// double d = s._a[r][c];
			// if( d != 0 ){
			    
			// }
			for( size_t j = 0; j < s._m; ++j ){
			    s._c_aux[j] -= s._a[r][j];
			}
			s._d_aux -= s._b[r];
		    }
		}
	    }
	}
	// }
	// cout << "aux obj func coeff: ";
	// for( auto & i : s._c_aux ){
	//     cout << i << " ";
	// }
	// cout << s._d_aux << endl;

	    // cout << "after aux init" << endl;

	    // print_aux( s );	
		
	if( s._num_aux > 0 ){
	    while(true){
		auto ret = compute_next_pivot_aux( s );
		if( ret.first == 0 ){ //basic feasible solution found
		    // size_t index_r = 0;
		    // for( auto & i : s._a ){
		    // 	for( auto & j : i ){
		    // 	    cout << j << " ";
		    // 	}
		    // 	cout << " " << s._b[index_r++] << endl;
		    // }
		    // cout << "non-basic: ";
		    // for( auto & i : s._non_basic ){
		    // 	cout << i << " ";
		    // }
		    // cout << endl;

		    // cout << "basic: ";
		    // for( auto & i : s._basic ){
		    // 	cout << i << " ";
		    // }
		    // cout << "-------" << endl;
		    
		    break;
		} else if( ret.first == 1 ){
		    return { ::e2::states::lp::e_lp_ans::NONE, {} }; //no solution
		} else if( ret.first == 3 ){
		    return { ::e2::states::lp::e_lp_ans::UNBOUNDED, {} };
		} else { //further optimization possible
		    int r = ret.second.first;
		    int c = ret.second.second;
		    gaussian_elimination_aux( s, ret.second );

		    // cout << "gaussian elimination ---- pivot: " << r << ", " << c << endl;

		    // size_t index_r = 0;
		    // for( auto & i : s._a ){
		    // 	for( auto & j : i ){
		    // 	    cout << j << " ";
		    // 	}
		    // 	cout << " " << s._b[index_r++] << endl;
		    // }
			
		    // cout << "non-basic: ";
		    // for( auto & i : s._non_basic ){
		    // 	cout << i << " ";
		    // }
		    // cout << endl;

		    // cout << "basic: ";
		    // for( auto & i : s._basic ){
		    // 	cout << i << " ";
		    // }
		    // cout << endl;
		}
	    }
	}

	// cout << "remove auxilliary variables" << endl;

	int count_pop = 0;
	for( size_t i = s._m - s._num_aux; i < s._m; ++i ){
	    s._non_basic.erase(i);
	    s._basic.erase(i);
	    count_pop++;
	    for( size_t j = 0; j < s._a.size(); ++j ){
		s._a[j].pop_back();
	    }
	}
	s._m -= s._num_aux;

	// cout << "obj coeff update: " << endl;
	// for( auto h : s._c ){
	//     cout << h << " ";
	// }
	// cout << endl;
	    
	// cout << "count_pop: " << count_pop << endl;
	
	//eliminate variable in objective function as well
	if( s._num_aux > 0 ){
	    for( size_t row = 0; row < s._n; ++row ){
		int col = s._map_row_to_basic[ row ];
		double d = s._c[col] / s._a[row][col]; //normalization factor
		// cout << "norm factor: " << d << endl;
		for( size_t i = 0; i < s._m; ++i ){
		    s._c[i] -= d * s._a[row][i];
		}
		s._d -= d * s._b[row];
	    }
	}
	s._num_aux = 0;

	// cout << "phase 2" << endl;

	//phase 2
	while(true){
	    auto ret = compute_next_pivot( s );
	    if( ret.first == 0 ){ //optimal found
		break;
	    } else if( ret.first == 1 ){
		// print( s );
		return { ::e2::states::lp::e_lp_ans::NONE, {} }; //no solution
	    } else if( ret.first == 3 ){
		// print( s );
		return { ::e2::states::lp::e_lp_ans::UNBOUNDED, {} };
	    } else { //further optimization possible
		int r = ret.second.first;
		int c = ret.second.second;
		gaussian_elimination( s, ret.second );

		// print( s );
		// cout << "gaussian elimination ----" << endl;
		// cout << "pivot: " << r << ", " << c << endl;

		// size_t index_r = 0;
		// for( auto & i : s._a ){
		//     for( auto & j : i ){
		// 	cout << j << " ";
		//     }
		//     cout << " " << s._b[index_r++] << endl;
		// }
			
		// cout << "non-basic: ";
		// for( auto & i : s._non_basic ){
		//     cout << i << " ";
		// }
		// cout << endl;

		// cout << "basic: ";
		// for( auto & i : s._basic ){
		//     cout << i << " ";
		// }
		// cout << endl;
	    }
	}

	//get values of the original variables
	std::vector<double> v( s._m_original, 0.0 );
	for( auto i : s._basic ){
	    if( i < s._m_original ){
		int row = s._map_basic_to_row[i];
		v[i] = s._b[row];
	    }
	}

	// for( auto i : v ){
	//     cout << i << " ";
	// }
	// cout << endl;

	return { ::e2::states::lp::e_lp_ans::BOUNDED, v };
    }
    static void print( simplex_standard & s ){

	std::cout << "----" << std::endl;
	std::cout << "obj func coeff: ";
	for( auto & i : s._c ){
	    std::cout << i << " ";
	}
	std::cout << "| " << s._d << std::endl;
	
	size_t index_r = 0;
	for( auto & i : s._a ){
	    for( auto & j : i ){
		std::cout << j << " ";
	    }
	    std::cout << " | " << s._b[index_r++] << std::endl;
	}
			
	std::cout << "non-basic: ";
	for( auto & i : s._non_basic ){
	    std::cout << i << " ";
	}
	std::cout << std::endl;

	std::cout << "basic: ";
	for( auto & i : s._basic ){
	    std::cout << i << " ";
	}
	std::cout << std::endl;
	std::cout << "----" << std::endl;
    }

    static void print_aux( simplex_standard & s ){

	std::cout << "----" << std::endl;
	std::cout << "obj func coeff: ";
	for( auto & i : s._c_aux ){
	    std::cout << i << " ";
	}
	std::cout << "| " << s._d_aux << std::endl;
	
	size_t index_r = 0;
	for( auto & i : s._a ){
	    for( auto & j : i ){
		std::cout << j << " ";
	    }
	    std::cout << " | " << s._b[index_r++] << std::endl;
	}
			
	std::cout << "non-basic: ";
	for( auto & i : s._non_basic ){
	    std::cout << i << " ";
	}
	std::cout << std::endl;

	std::cout << "basic: ";
	for( auto & i : s._basic ){
	    std::cout << i << " ";
	}
	std::cout << std::endl;
	std::cout << "----" << std::endl;
    }
};
	
class lp_simplex_std final : public ::e2::interface::i_lp< simplex_standard > {
public:
    static bool init( simplex_standard & s, std::vector< std::vector< double > > a, std::vector< double > b, std::vector< double > c ){
	return simplex_standard::init( s, a, b, c );
    }
    static std::pair< ::e2::states::lp::e_lp_ans, std::vector<double> > solve( simplex_standard & s ){
	return simplex_standard::solve( s );
    }
};

} }

#endif
