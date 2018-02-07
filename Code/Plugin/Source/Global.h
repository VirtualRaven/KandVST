#ifndef GLOBAL_H
#define GLOBAL_H

class ParameterHandler;
class Log;
struct GLOBAL {
	ParameterHandler* paramHandler;
	Log* log;

	//Fix destructor
};
extern GLOBAL Global;

#endif // !GLOBAL_H
