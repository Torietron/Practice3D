/***** MousePoll.h *****/
#ifndef DEF_MOUSEPOLL_H
#define DEF_MOUSEPOLL_H

#include <cstdint>
class MousePoll {
	public:
		int Poll[4];
		int_fast16_t x, y;
		MousePoll(float decay = 0.55f);
		int Update(uint_fast16_t rate = 10);
		bool Moved();
		float GetDeltaX(uint_fast8_t type = 0);
		float GetDeltaY(uint_fast8_t type = 0);
		void Reset(int_fast16_t a, int_fast16_t b);
	private:
		const float SMOOTH_DECAY;
		int_fast16_t last_x, last_y;
		float delta_x, delta_y;
		float posdelta_x, posdelta_y;
};

#endif