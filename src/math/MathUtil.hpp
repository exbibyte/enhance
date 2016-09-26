#ifndef MATH_UTIL_H
#define MATH_UTIL_H

class math_util {
public:
    long long exponentiation_mod( long long base, long long exp, long long mod ){
	long res=1;
	while(exp>0) {
	    if(exp%2==1) res=(res*base) % mod;
	    base=(base*base) % mod;
	    exp/=2;
	}
	return res % mod;
    }
};

#endif
