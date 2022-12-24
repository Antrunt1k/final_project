#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono> //Библиотека для работы с временем
#include <thread> //Библиотека для работы с потоками
#include <Game.hpp>

int main()
{
	mt::Game game;
	game.SetResolution(640, 480);
	game.SetCaption("GAME");
	game.Setup();

	game.Run();

	return 0;
}