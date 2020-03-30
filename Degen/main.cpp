//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//
//#ifdef _DEBUG   
//#ifndef DBG_NEW      
//#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )     
//#define new DBG_NEW   
//#endif
//#endif

#include "Engine/cEngine.h"

int main(int argc, char* argv[])
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	
	Degen::cEngine engine;
	if (!engine.Initialize("init.json")) return EXIT_FAILURE;
	if (!engine.Load("data.json")) return EXIT_FAILURE;
	engine.Go();
	engine.CleanUp();
}
