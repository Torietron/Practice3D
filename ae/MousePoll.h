/***** MousePoll.h *****/
#ifndef DEF_MOUSEPOLL_H
#define DEF_MOUSEPOLL_H

#include <cstdint>
class MousePoll {
	public:
		int Poll[4];
		int_fast16_t x, y;
		MousePoll();
		int Update();
};

#endif