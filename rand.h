const int MAX_GEN = 1000;

class Rand
{
public:
	//Rand();

	static bool rand();

private:
	void check_seed();
	
	static unsigned int count;
};
