#include "Tplai.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	Tplai tplai;
	while (tplai.running()) {
		tplai.update();
		tplai.render();
	}

	return 0;
}
