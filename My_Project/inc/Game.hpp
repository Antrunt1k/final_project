#pragma once
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <Ball.hpp>

namespace mt
{
    class Game
    {
    private:
        int m_width, m_height;
        std::string m_caption;
        sf::RenderWindow* m_window = nullptr; //Указатель на окно, которому присваевается nullptr (нулевой показатель)
        sf::Clock m_timer;                    //Запуск одного таймера на весь процесс анимации
    public:
        Game(){}

        ~Game();

        void SetResolution(int width, int height);   //Установка разрешения экрана

        void SetCaption(const std::string& caption); //Установка заголовка

        void Setup();                                //Функция, создающая окно

        void Run();
    };
}
