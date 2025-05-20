#include <SFML/Graphics.hpp>
using namespace sf;
class Zombie
{
private:
	// How fast is each zombie type?
	const float BLOATER_SPEED =10;     
	const float CHASER_SPEED = 20;       
	const float CRAWLER_SPEED = 5;  
	// How tough is each zombie type
	const float BLOATER_HEALTH = 3;
	const float CHASER_HEALTH = 1;
	const float CRAWLER_HEALTH = 2;
	Vector2f m_Position;// Where is this zombie?
	Texture m_Texture;// A sprite for the zombie
	Sprite m_Sprite;
	// How fast can this one run/crawl?
	float m_Speed;
	// How much health has it got?
	float m_Health;
	// Is it still alive?
	bool m_Alive;	
public:
    FloatRect getPosition();
	Sprite getSprite();
	void spawn(float startX, float startY, int type, int seed);
	void update(float elapsedTime, Vector2f playerLocation);
	bool hit();// Handle when a bullet hits a zombie
	bool isAlive();// Find out if the zombie is alive
	
};
FloatRect Zombie::getPosition(){
	return m_Sprite.getGlobalBounds();
}
Sprite Zombie::getSprite(){
	return m_Sprite;
}
void Zombie::spawn(float startX, float startY, int type, int seed)
{
   switch (type)
	{
	case 0:// Bloater
	    m_Texture.loadFromFile("graphics/bloater.png");
		m_Sprite.setTexture(m_Texture);
		m_Speed = BLOATER_SPEED;
		m_Health = BLOATER_HEALTH;
        m_Alive=true; 
		break;

	case 1:// Chaser
	    m_Texture.loadFromFile("graphics/chaser.png");
		m_Sprite.setTexture(m_Texture);
		m_Speed = CHASER_SPEED;
		m_Health = CHASER_HEALTH;
		m_Alive=true;
		break;
	case 2:// Crawler
	    m_Texture.loadFromFile("graphics/crawler.png");
		m_Sprite.setTexture(m_Texture);
		m_Speed = CRAWLER_SPEED;
		m_Health = CRAWLER_HEALTH;
		m_Alive=true;
		break;
	}
	// Modify the speed to make the zombie unique. Every zombie is unique. Create a speed modifier
	srand((int)time(0) * seed);
	float modifier = rand() % (101-70) + 70;// Somewhere between 70 an 100
	modifier /= 100; // Now equals between .7 and 1
	m_Speed *= modifier;
	m_Position.x = startX;
	m_Position.y = startY;
	m_Sprite.setOrigin(25, 25);
	m_Sprite.setPosition(m_Position);
}
void Zombie::update(float elapsedTime,Vector2f playerLocation)
{
	float playerX = playerLocation.x;
	float playerY = playerLocation.y;
	// Update the zombie position variables
	if (playerX > m_Position.x){
		m_Position.x = m_Position.x +m_Speed * elapsedTime;
	}
	if (playerY > m_Position.y){
		m_Position.y = m_Position.y +m_Speed * elapsedTime;
	}
	if (playerX < m_Position.x){
		m_Position.x = m_Position.x -m_Speed * elapsedTime;
	}
	if (playerY < m_Position.y){
		m_Position.y = m_Position.y - m_Speed * elapsedTime;
	}
	m_Sprite.setPosition(m_Position);
	// Face the sprite in the correct direction
	float angle = (atan2(playerY - m_Position.y, playerX - m_Position.x)*180) / 3.141;
	m_Sprite.setRotation(angle);
}
bool Zombie::hit()
{
	m_Health--;
	if (m_Health <=0){
		m_Alive = false;	// dead
	    m_Texture.loadFromFile("graphics/blood.png");
		m_Sprite.setTexture(m_Texture);
		return true;
	}
	return false;// injured but not dead yet
}

bool Zombie::isAlive()
{
	return m_Alive;
}




