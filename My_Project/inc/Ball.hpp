#pragma once

namespace mt
{
	struct Point{float x, y;}; //Структура: точка
	struct Vec  {float x, y;}; //Структура: скорость

	class Ball
	{
	private:
		Point m_p0;				  //Точка
		Vec m_velocity;			  //Скорость
		float m_R;				  //Радиус
		sf::CircleShape* m_shape; //Указатель на круг
		sf::Texture* m_texture;
		sf::Sprite* m_sprite = nullptr;
		float m_scale;
	public:
		Ball(Point p0, Vec v, sf::Color color)			 //Конструктор, где происходит создание круга. На вход принимаются начальные параметры
		{
			m_p0 = p0;									 //Начальная точка, включающая координаты x, y, получаемые из strucr Point
			m_velocity = v;								 //Скорость круга				
		}

		bool Setup(const std::string& name, float scale) //&-ссылка на строку //Const - запрещает изменения внутри цикла
		{
			m_texture = new sf::Texture();			     //Создание текстуры
			if (!m_texture->loadFromFile(name))		     //Проверка - загружена ли текстура
			{
				std::cout << "Error" << std::endl;
				return false;
			}
			m_sprite = new sf::Sprite;				     //Создание спрайта
			m_sprite->setTexture(*m_texture);		     //Установка текстуры для спрайта
			float x = m_texture->getSize().x;
			float y = m_texture->getSize().y;
			m_scale = scale;
			m_sprite->setScale(m_scale, m_scale);	     //Изменение размера объекта	
			m_sprite->setOrigin(x/2, y/2);			     //Смещение системы координат объекта в его центр
			m_R = m_texture->getSize().x*m_scale/2;
			return true;
		}

		~Ball() //Удаление после конструктора, того, что занимает память 
		{
			if (m_texture != nullptr)
				delete m_texture;
			if (m_sprite != nullptr)
				delete m_sprite;
		}

		void Move(double deltaTime) //Функция, описывающая движение
		{
			m_p0.x += m_velocity.x * deltaTime; //Координата центра шарика X
			m_p0.y += m_velocity.y * deltaTime; //Координата центра шарика Y
			m_sprite->setPosition(m_p0.x, m_p0.y);
		}

		sf::Sprite* Get() { return m_sprite; }

		void SetVelocity(Vec v) //Функция для изменения скорости
		{
			m_velocity = v;
		}

		Point GetPosition() { return m_p0; }	 //Получение текущей позиции центра объекта
		Vec GetVelocity() { return m_velocity; } //Получение текущей скорости
		float Radius() { return m_R; }			 //Получение радиуса
	};
}