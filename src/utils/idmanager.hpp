#ifndef __IDMANAGER_HPP__
#define __IDMANAGER_HPP__

class IdMng {
private:
	static int currId;

public:
	static int Next() {
		if (currId > MAX_QUADS) {
			currId--;
		}
		return currId++;
	}

	static void Use(int num) {
		currId = num;
	}
};

int IdMng::currId = 0;

#endif