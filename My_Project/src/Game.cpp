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

    void Game::SetResolution(int width, int height) //Установка разрешения экрана
    {
        m_width = width; m_height = height;
    }

    void Game::SetCaption(const std::string& caption) //Установка заголовка
    {
        m_caption = caption;
    }

    void Game::Setup() //Функция, создающая окно
    {
        m_window = new sf::RenderWindow(sf::VideoMode(m_width, m_height), m_caption);
    }

    void Game::Run() //Основная функция процесса
    {
        sf::Music music;                                                       //Создание объекта музыки
        music.openFromFile("assets/Track.wav");                                       //Загрузка файла
        music.setVolume(50.f);                                                 //Громкость
        music.play();                                                          //Воспроизведение музыку

        std::vector<mt::Ball*> balls;                                          //vector - динамический массив
        mt::Ball* ball = new mt::Ball({ 64,64 }, { 48,100 }, sf::Color::Blue); //({x,y},{Vx,Vy}, R) //Создание круга 
        if (!ball->Setup("assets/image.png", 0.25))                                   //Если произошла ошибка в Setup'е, то метод Run не запустится
            return;
        balls.push_back(ball);                                                 //push_back - добавление элемента в вектор

        ball = new mt::Ball({ 576,64 }, { 90, 100 }, sf::Color::Cyan);         //Втрой объект
        if (!ball->Setup("assets/image2.png", 0.25))
            return;
        balls.push_back(ball);


        m_timer.restart();         //Таймер обнуляется каждый раз, когда начинается цикл
        while (m_window->isOpen()) //Основной цикл
        {
            sf::Event event;
            while (m_window->pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    m_window->close();
            }

            for (int i = 0; i < balls.size(); i++)        //Цикл изменения направления движения //Расчёт соударений
            {
                Point p = balls[i]->GetPosition();        //Текущая позиция
                float R = balls[i]->Radius();             //Текущий радиус

                if (p.y - R < 0)                          //Верхний край
                {
                    Vec v = balls[i]->GetVelocity();
                    balls[i]->SetVelocity({ v.x, -v.y });
                }
                if (p.x - R < 0)                          //Левый край
                {
                    Vec v = balls[i]->GetVelocity();
                    balls[i]->SetVelocity({ -v.x, v.y });
                }
                if (p.x + R > m_width)                    //Правый край
                {
                    Vec v = balls[i]->GetVelocity();
                    balls[i]->SetVelocity({ -v.x, v.y });
                }
                if (p.y + R > m_height)                   //Нижний край
                {
                    Vec v = balls[i]->GetVelocity();      //Текущая скорости
                    balls[i]->SetVelocity({ v.x, -v.y }); //Изменение скорости
                }                                                                                                           
            }

            mt::Ball*& smile = balls[1];
            mt::Ball*& cool = balls[0];

            float smilerad = smile->Radius();
            float collrad = cool->Radius();
            Vec smilevelocity = smile->GetVelocity();
            Vec coolvelocity = cool->GetVelocity();

            mt::Point sr = smile->GetPosition(); //Центр объекта
            mt::Point cr = cool->GetPosition();  //Центр объекта
            
            float d = sqrt(pow((sr.x - cr.x), 2) + pow((sr.y - cr.y), 2)); if (d == 0) d = 0.001; //Расстояние между центрами объектов
            float sinus = (sr.x - cr.x) / d;
            float cosinus = (sr.y - cr.y) / d;

            if (d < smilerad + collrad) //Цикл изменения поведения объектов при их столкновении
            {
                
                //Разворачиваем систему координат относительно центров объектов и получаем скорости
                float os_x_of_smile = sinus * coolvelocity.x + cosinus * coolvelocity.y;
                float os_x_of_cool = sinus * smilevelocity.x + cosinus * smilevelocity.y;
                float os_y_of_smile = -cosinus * coolvelocity.x + sinus * coolvelocity.y;
                float os_y_of_cool = -cosinus * smilevelocity.x + sinus * smilevelocity.y;

                //Меняем скорости объектов
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

                sf::Time deltaTime = m_timer.restart();    //Получение времени выполнения цикла, и последующее обнуление таймера   
                                                                                                                                
                for (int i = 0; i < balls.size(); i++)     //Движение объектов в одном цикле                                            
                    balls[i]->Move(deltaTime.asSeconds()); //Движение

                m_window->clear();
                for (int i = 0; i < balls.size(); i++)     //Отрисовка объектов в одном цикле
                    m_window->draw(*balls[i]->Get());
                m_window->display();

                std::this_thread::sleep_for(std::chrono::milliseconds(30)); //Ограничение частоты обновление (Hz), дабы снизить загрузку ЦП
            }

            for (int i = 0; i < balls.size(); i++) //Удаление элементов для очистки памяти
                delete balls[i];
        };
    }
