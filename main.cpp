#include "Tplai.h"

int WinMain()
{

	Tplai tplai;
	while (tplai.running()) {
		tplai.update();
		tplai.render();
	}

	return 0;
}
