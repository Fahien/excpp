#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>

using namespace std;

class Say {
public:
	Say(const string& _text) :text{ _text } {}

	void operator()()
	{
		while (true) {
			cout << text << endl;
			this_thread::sleep_for(chrono::milliseconds{ 1000 });
		}
	}

private:
	const string& text;
};

/// Program with two threads
/// one that writes hello every second and one that writes world every second
int main()
{
	thread t1{ Say{ "Hello" } };
	thread t2{ Say{ "World" } };

	this_thread::sleep_for(chrono::milliseconds{ 5000 });
	return EXIT_SUCCESS;
}
