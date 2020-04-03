#ifndef _TEST_UTILS_H
#define _TEST_UTILS_H

#include <iostream>

#define TEST(cond) \
	if (!(cond)) { \
		std::cout << "TEST FAILED (" << #cond << ")\n"; \
	} else { \
		std::cout << "TEST PASSED\n"; \
	}

#endif