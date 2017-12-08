#ifndef __GENERAL_H_
#define __GENERAL_H_

#include <cstdlib>

float randf(float min, float max) {
	return min + (rand() / (RAND_MAX / (max - min)));
}

#endif