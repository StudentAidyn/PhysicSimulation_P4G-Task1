#define _CRTDBG_MAP_ALLOC
#include "PhysicsSceneApp.h"
#include <crtdbg.h>

int main() {
	//https://www.youtube.com/watch?v=t1wqj6J6Vhs&ab_channel=CodeVault
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// allocation
	auto app = new PhysicsSceneApp();

	// initialise and loop
	app->run("AIE", 1280, 720, false);

	// deallocation
	delete app;

	return 0;
}