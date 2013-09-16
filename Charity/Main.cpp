#include "Engine.h"

Engine* engine;

int main()
{
	engine = new Engine();
	engine->Begin();
	engine->Tick();
	engine->End();
};