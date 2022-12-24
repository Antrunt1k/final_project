#include <Game.hpp>
#include <thread>
#include <chrono>
#include <SFML/Audio.hpp>

namespace mt
{
    Game::~Game()
    {
        if (m_window != nullptr)
            delete m_window;
    }

    void Game::SetResolution(int width, int height) //��������� ���������� ������
    {
        m_width = width; m_height = height;
    }

    void Game::SetCaption(const std::string& caption) //��������� ���������
    {
        m_caption = caption;
    }

    void Game::Setup() //�������, ��������� ����
    {
        m_window = new sf::RenderWindow(sf::VideoMode(m_width, m_height), m_caption);
    }

    void Game::Run() //�������� ������� ��������
    {
        sf::Music music;                                                       //�������� ������� ������
        music.openFromFile("assets/Track.wav");                                       //�������� �����
        music.setVolume(50.f);                                                 //���������
        music.play();                                                          //��������������� ������

        std::vector<mt::Ball*> balls;                                          //vector - ������������ ������
        mt::Ball* ball = new mt::Ball({ 64,64 }, { 48,100 }, sf::Color::Blue); //({x,y},{Vx,Vy}, R) //�������� ����� 
        if (!ball->Setup("assets/image.png", 0.25))                                   //���� ��������� ������ � Setup'�, �� ����� Run �� ����������
            return;
        balls.push_back(ball);                                                 //push_back - ���������� �������� � ������

        ball = new mt::Ball({ 576,64 }, { 90, 100 }, sf::Color::Cyan);         //����� ������
        if (!ball->Setup("assets/image2.png", 0.25))
            return;
        balls.push_back(ball);


        m_timer.restart();         //������ ���������� ������ ���, ����� ���������� ����
        while (m_window->isOpen()) //�������� ����
        {
            sf::Event event;
            while (m_window->pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    m_window->close();
            }

            for (int i = 0; i < balls.size(); i++)        //���� ��������� ����������� �������� //������ ����������
            {
                Point p = balls[i]->GetPosition();        //������� �������
                float R = balls[i]->Radius();             //������� ������

                if (p.y - R < 0)                          //������� ����
                {
                    Vec v = balls[i]->GetVelocity();
                    balls[i]->SetVelocity({ v.x, -v.y });
                }
                if (p.x - R < 0)                          //����� ����
                {
                    Vec v = balls[i]->GetVelocity();
                    balls[i]->SetVelocity({ -v.x, v.y });
                }
                if (p.x + R > m_width)                    //������ ����
                {
                    Vec v = balls[i]->GetVelocity();
                    balls[i]->SetVelocity({ -v.x, v.y });
                }
                if (p.y + R > m_height)                   //������ ����
                {
                    Vec v = balls[i]->GetVelocity();      //������� ��������
                    balls[i]->SetVelocity({ v.x, -v.y }); //��������� ��������
                }                                                                                                           
            }

            mt::Ball*& smile = balls[1];
            mt::Ball*& cool = balls[0];

            float smilerad = smile->Radius();
            float collrad = cool->Radius();
            Vec smilevelocity = smile->GetVelocity();
            Vec coolvelocity = cool->GetVelocity();

            mt::Point sr = smile->GetPosition(); //����� �������
            mt::Point cr = cool->GetPosition();  //����� �������
            
            float d = sqrt(pow((sr.x - cr.x), 2) + pow((sr.y - cr.y), 2)); if (d == 0) d = 0.001; //���������� ����� �������� ��������
            float sinus = (sr.x - cr.x) / d;
            float cosinus = (sr.y - cr.y) / d;

            if (d < smilerad + collrad) //���� ��������� ��������� �������� ��� �� ������������
            {
                
                //������������� ������� ��������� ������������ ������� �������� � �������� ��������
                float os_x_of_smile = sinus * coolvelocity.x + cosinus * coolvelocity.y;
                float os_x_of_cool = sinus * smilevelocity.x + cosinus * smilevelocity.y;
                float os_y_of_smile = -cosinus * coolvelocity.x + sinus * coolvelocity.y;
                float os_y_of_cool = -cosinus * smilevelocity.x + sinus * smilevelocity.y;

                //������ �������� ��������
                float a = os_x_of_cool;
                os_x_of_cool = os_x_of_smile;
                os_x_of_smile = a;

                smilevelocity.x= os_x_of_cool * sinus - os_y_of_cool * cosinus;
                smilevelocity.y = os_x_of_cool * cosinus + os_y_of_cool * sinus;
                coolvelocity.x = os_x_of_smile * sinus - os_y_of_smile * cosinus;
                coolvelocity.y = os_x_of_smile * cosinus + os_y_of_smile * sinus;

                smile->SetVelocity({ smilevelocity.x, smilevelocity.y });
                cool->SetVelocity({ coolvelocity.x,coolvelocity.y });
            }

                sf::Time deltaTime = m_timer.restart();    //��������� ������� ���������� �����, � ����������� ��������� �������   
                                                                                                                                
                for (int i = 0; i < balls.size(); i++)     //�������� �������� � ����� �����                                            
                    balls[i]->Move(deltaTime.asSeconds()); //��������

                m_window->clear();
                for (int i = 0; i < balls.size(); i++)     //��������� �������� � ����� �����
                    m_window->draw(*balls[i]->Get());
                m_window->display();

                std::this_thread::sleep_for(std::chrono::milliseconds(30)); //����������� ������� ���������� (Hz), ���� ������� �������� ��
            }

            for (int i = 0; i < balls.size(); i++) //�������� ��������� ��� ������� ������
                delete balls[i];
        };
    }
