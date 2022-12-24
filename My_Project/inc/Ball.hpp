#pragma once

namespace mt
{
	struct Point{float x, y;}; //���������: �����
	struct Vec  {float x, y;}; //���������: ��������

	class Ball
	{
	private:
		Point m_p0;				  //�����
		Vec m_velocity;			  //��������
		float m_R;				  //������
		sf::CircleShape* m_shape; //��������� �� ����
		sf::Texture* m_texture;
		sf::Sprite* m_sprite = nullptr;
		float m_scale;
	public:
		Ball(Point p0, Vec v, sf::Color color)			 //�����������, ��� ���������� �������� �����. �� ���� ����������� ��������� ���������
		{
			m_p0 = p0;									 //��������� �����, ���������� ���������� x, y, ���������� �� strucr Point
			m_velocity = v;								 //�������� �����				
		}

		bool Setup(const std::string& name, float scale) //&-������ �� ������ //Const - ��������� ��������� ������ �����
		{
			m_texture = new sf::Texture();			     //�������� ��������
			if (!m_texture->loadFromFile(name))		     //�������� - ��������� �� ��������
			{
				std::cout << "Error" << std::endl;
				return false;
			}
			m_sprite = new sf::Sprite;				     //�������� �������
			m_sprite->setTexture(*m_texture);		     //��������� �������� ��� �������
			float x = m_texture->getSize().x;
			float y = m_texture->getSize().y;
			m_scale = scale;
			m_sprite->setScale(m_scale, m_scale);	     //��������� ������� �������	
			m_sprite->setOrigin(x/2, y/2);			     //�������� ������� ��������� ������� � ��� �����
			m_R = m_texture->getSize().x*m_scale/2;
			return true;
		}

		~Ball() //�������� ����� ������������, ����, ��� �������� ������ 
		{
			if (m_texture != nullptr)
				delete m_texture;
			if (m_sprite != nullptr)
				delete m_sprite;
		}

		void Move(double deltaTime) //�������, ����������� ��������
		{
			m_p0.x += m_velocity.x * deltaTime; //���������� ������ ������ X
			m_p0.y += m_velocity.y * deltaTime; //���������� ������ ������ Y
			m_sprite->setPosition(m_p0.x, m_p0.y);
		}

		sf::Sprite* Get() { return m_sprite; }

		void SetVelocity(Vec v) //������� ��� ��������� ��������
		{
			m_velocity = v;
		}

		Point GetPosition() { return m_p0; }	 //��������� ������� ������� ������ �������
		Vec GetVelocity() { return m_velocity; } //��������� ������� ��������
		float Radius() { return m_R; }			 //��������� �������
	};
}