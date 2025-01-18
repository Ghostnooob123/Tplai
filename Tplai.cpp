// Tplai.cpp : Defines the entry point for the application.
//

#include "Tplai.h"

int main()
{

	Frontend tplay;
	while (tplay.running()) {
		tplay.update();
		tplay.render();
	}

	return 0;
}
