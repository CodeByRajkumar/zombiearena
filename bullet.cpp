#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;
class Bullet{
private:
	Vector2f m_Position;// Where is the bullet?
	RectangleShape m_BulletShape;// What each bullet looks like
	bool m_InFlight = false;// Is this bullet currently whizzing through the air
	float m_BulletSpeed = 1000;// How fast does a bullet travel?
	float m_BulletSpeedX;
	float m_BulletSpeedY;
	// Some boundaries so the bullet doesn't fly forever
	float m_MaxX;
	float m_MinX;
	float m_MaxY;
	float m_MinY;
public:
	Bullet(); 
    FloatRect getPosition();
	RectangleShape getShape();
	void shoot(float startX, float startY,float xTarget, float yTarget); // Launch a new bullet
    void update(float elapsedTime);
    void stop();// Stop the bullet
	bool isInFlight();
};
Bullet::Bullet()
{
	m_BulletShape.setSize(sf::Vector2f(20,20));
	m_BulletShape.setFillColor(Color::Red);
}
FloatRect Bullet::getPosition(){
	return m_BulletShape.getGlobalBounds();
}
RectangleShape Bullet::getShape(){
	return m_BulletShape;
}
void Bullet::shoot(float startX, float startY,float targetX, float targetY)
{
	m_InFlight = true;// Keep track of the bullet
	m_Position.x = startX;
	m_Position.y = startY;
	// Calculate the gradient of the flight path
    float gradient = (targetY - startY) / (targetX-startX);
	// Any gradient less than zero needs to be negative
	if (gradient < 0){
		gradient *= -1;
	}
	// Set the "speed" horizontally and vertically
	m_BulletSpeedX = m_BulletSpeed / (1 + gradient);
	m_BulletSpeedY = m_BulletSpeed*gradient / (1 + gradient);
	// Point the bullet in the right direction
	if (targetX < startX){
		m_BulletSpeedX *= -1;
	}
	if (targetY < startY){
		m_BulletSpeedY *= -1;
	}
	// Set a max range of 1000 pixels
	float range = 1000;
	m_MinX = startX - range;
	m_MaxX = startX + range;
	m_MinY = startY - range;
	m_MaxY = startY + range;
	// Position the bullet ready to be drawn
	m_BulletShape.setPosition(m_Position);
}
void Bullet::update(float elapsedTime)
{
	// Update the bullet position variables
	m_Position.x += m_BulletSpeedX * elapsedTime;
	m_Position.y += m_BulletSpeedY * elapsedTime;
	// Move the bullet
	m_BulletShape.setPosition(m_Position);
	// Has the bullet gone out of range?
	if (m_Position.x < m_MinX || m_Position.x > m_MaxX ||
		m_Position.y < m_MinY || m_Position.y > m_MaxY)
	{
		m_InFlight = false;
	}
}
void Bullet::stop()
{
	m_InFlight = false;
}

bool Bullet::isInFlight()
{
	return m_InFlight;
}




