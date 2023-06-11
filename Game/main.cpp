#include "pch.h"
#include <ctime>

#include "constants.h"
#include "EntityKeeper.h"
#include "Game.h"
#include "GhostsAndGoblins.h"


void StartHeapControl();
void DumpMemoryLeaks();

int SDL_main(int argv, char** args)
{
	srand(static_cast<unsigned int>(time(nullptr)));

	StartHeapControl();

	Game* pGame{ new GhostsAndGoblins{ Window{
		"Ghosts N' Goblins - Nellessen, Patrick - 1DAE12",
		constants::SCREEN_WIDTH,
		constants::SCREEN_HEIGHT,
		false
	} } };
	pGame->InitializeGameEngine();
	pGame->Run();
	delete pGame;
	DumpMemoryLeaks();
	return 0;
}


void StartHeapControl()
{
#if defined(DEBUG) | defined(_DEBUG)
	// Notify user if heap is corrupt
	HeapSetInformation(nullptr, HeapEnableTerminationOnCorruption, nullptr, 0);

	// Report detected leaks when the program exits
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Set a breakpoint on the specified object allocation order number
	//_CrtSetBreakAlloc( 156 );
#endif
}

void DumpMemoryLeaks()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif
}
