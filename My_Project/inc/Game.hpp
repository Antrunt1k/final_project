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
        sf::RenderWindow* m_window = nullptr; //��������� �� ����, �������� ������������� nullptr (������� ����������)
        sf::Clock m_timer;                    //������ ������ ������� �� ���� ������� ��������
    public:
        Game(){}

        ~Game();

        void SetResolution(int width, int height);   //��������� ���������� ������

        void SetCaption(const std::string& caption); //��������� ���������

        void Setup();                                //�������, ��������� ����

        void Run();
    };
}
