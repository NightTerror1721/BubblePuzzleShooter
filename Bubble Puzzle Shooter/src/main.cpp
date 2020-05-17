#include <iostream>

#include "memory.h"
#include "game.h"
#include "manager.h"
#include "bubble.h"
#include "props.h"

int main(int argc, char** argv)
{
	GameController gc{ "Bubble Puzzle Shooter" };
	gc.setStyle(WindowStyle::Default);
	gc.start();

	std::cout << Properties::getString("testprop", "nope") << std::endl;

	return 0;
}
