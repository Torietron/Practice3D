/***** MousePoll.h *****/
#ifndef DEF_MOUSEPOLL_H
#define DEF_MOUSEPOLL_H

#include <cstdint>
class MousePoll {
	public:
		int Poll[4];
		int_fast16_t x, y;
		MousePoll();
		int Update(uint_fast16_t rate = 5);
		bool Moved();
		int_fast16_t GetDeltaX();
		int_fast16_t GetDeltaY();
		void Reset(int_fast16_t a, int_fast16_t b);
	private:
		int_fast16_t last_x, last_y;
		int_fast16_t delta_x, delta_y;
		int_fast16_t distance_x, distance_y;
};

#endif