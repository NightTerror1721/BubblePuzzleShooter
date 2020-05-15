#include <iostream>

#include "memory.h"
#include "game.h"
#include "manager.h"

int main(int argc, char** argv)
{
	GameController gc{ "Bubble Puzzle Shooter" };
	gc.setStyle(WindowStyle::Default);
	gc.start();

	return 0;
}
