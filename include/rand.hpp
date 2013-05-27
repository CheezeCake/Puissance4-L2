const unsigned int MAX_GEN = 1000;

class Rand
{
public:
	//Rand();

	static bool rand_bool();

private:
	static void check_seed();
	
	static unsigned int count;
};
