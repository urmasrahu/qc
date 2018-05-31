#include <stdlib.h>
#include <time.h>

class RandomizerInitializer
{
public:
	RandomizerInitializer()
	{
		srand(static_cast<unsigned int>(time(NULL)));
	}
};

static RandomizerInitializer randomizerInitializer;
