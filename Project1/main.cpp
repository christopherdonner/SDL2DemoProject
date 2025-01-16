#include <iostream>
#include <SDL.h>

using namespace std;



void testFunction() 
{
	cout << "this is the test function" << endl;
}

int main(void)
{
	int age = 43;
	cout << boolalpha;
	cout << (age >= 21) << endl;

	if (age < 65) {
		cout << "not yet retirement age"<<endl;
	}
	for (int i = 0; i < 10; i++) {
		cout << i << endl;
	}
	testFunction();

	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "hello world", "Successful", NULL);

	return 0;
}
