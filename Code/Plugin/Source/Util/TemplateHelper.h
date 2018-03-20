#ifndef TEMPLATE_HELPER
#define TEMPLATE_HELPER
#include <initializer_list>
#include <vector>
#include  <stdexcept>
	
template<typename F, unsigned int I> 
class parameterArguments : parameterArguments<F, I - 1> {
	F args;
public:

	parameterArguments(std::initializer_list<F> params) :
		parameterArguments<F, I - 1>(params.begin() + 1, params.size()-1),
		args(*params.begin()) {}

	parameterArguments(typename std::initializer_list<F>::iterator it,
		int size) :
		parameterArguments<F, I - 1>(it + 1,size-1),
		args(*it){}

	F current() { return args; }
	parameterArguments<F, I - 1> split() { return static_cast<parameterArguments<F, I - 1>>(*this);}
};

template<typename F> 
class parameterArguments<F, 0> {
public:
	parameterArguments(typename std::initializer_list<F>::iterator ,int size) {
		if (size != 0)
			throw std::length_error("Too few arguments provided");
	}
};


template<typename F, typename... argN> 
struct setParameters {};

template<typename F, typename arg0, typename... argN> 
struct setParameters<F, arg0, argN...> : setParameters<F, argN...> {
	
	setParameters(std::initializer_list<std::vector<F>> list) :
		setParameters<F, arg0, argN...>(parameterArguments<std::vector<F>, sizeof...(argN)+1>(list)) {}

	setParameters(parameterArguments<std::vector<F>, 1 + sizeof...(argN)> list) : setParameters<F, argN...>(list.split()) {
		for (auto it : list.current())
			arg0::RegisterParameters(it);
	}
};

template<typename F, typename arg0> 
struct setParameters<F, arg0> {
	setParameters(parameterArguments<std::vector<F>, 1> list) {
		for (auto it : list.current())
			arg0::RegisterParameters(it);
	}
};

template<typename T>
struct Constants {};

template<> struct Constants<double> {
	static const double PI;
};

template<> struct Constants<float> {
	static const float PI;
};

/* 
Test code

struct testStruct {
static void RegisterParameters(int i) {
std::cout << i << std::endl;

}
};

struct testStruct2 {
static void RegisterParameters1(int i) {
std::cout << i << std::endl;
}
};

// This will call testStruct::RegisterParameters four time, using 0 ,1 ,2 and 3 followed
// by two calls to testStruct2::RegisterParameters

setParameters<int, //Argument type
			  testStruct,    //First type to call RegisterParameter on
			  testStruct2> set({
									{0,1,2,3}, //Arguments to first type
									{0,1}  //Arguments to second type
							});

							

*/
#endif
