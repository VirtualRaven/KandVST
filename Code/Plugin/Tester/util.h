#ifndef UTIL_H_INCLUDED
#include <functional>
namespace util {
	void red(std::function<void()> f);
	void init_colour();
}
#endif // !UTIL_H_INCLUDED
