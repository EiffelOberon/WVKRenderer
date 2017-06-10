#include <iostream>

#ifdef _DEBUG
#define WASSERT(a, b)\
				{\
					assert(a);\
					std::cout << b << std::endl;\
				}
#else
#define WASSERT(a, b) {}
#endif