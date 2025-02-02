#include "Tplai.h"

int main()
{

	Tplai tplai;
	while (tplai.running()) {
		tplai.update();
		tplai.render();
	}

	return 0;
}
