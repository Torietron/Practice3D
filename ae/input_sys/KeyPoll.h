/***** KeyPoll.h *****/
#ifndef DEF_KEYPOLL_H
#define DEF_KEYPOLL_H

class KeyPoll {
	public:
		int Poll[256];
		KeyPoll();
		int Update();
};

#endif