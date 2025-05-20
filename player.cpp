#include<SFML/Graphics.hpp>
using namespace sf;
using namespace std;
#include <cmath>

class Player{
	private:
		const float START_SPEED=200;
		const float START_HEALTH=100;
		Vector2f m_Position;
		Texture m_Texture;
		Sprite m_Sprite;
		float m_Speed;
		int m_Health;
		int m_MaxHealth;
		Vector2f m_Resolution;
		int m_TileSize;
		IntRect m_Arena;
		bool m_UpPressed;// Which direction(s) is the player currently moving in
		bool m_DownPressed;
		bool m_LeftPressed;
		bool m_RightPressed;
		Time m_LastHit;  // When was the player last hit
	public:
		Player();
		FloatRect getPosition();
		Sprite getSprite();
		Vector2f getCenter();
		float getRotation();// what angle the player is facing
		void spawn(IntRect arena, Vector2f resolution, int tileSize);
		void moveRight(); //Functions to move the player
		void stopRight();
		void moveLeft();
		void stopLeft();
		void moveUp();
		void stopUp();
		void moveDown();
		void stopDown();
		void update(float elapsedTime,Vector2i musePosition);
        bool hit(Time timeHit); 
        int getHealth(); // How much health has the player currently got?
		void resetPlayerStats();	
		void upgradeSpeed(); // Give the player a speed boost
	    void upgradeHealth(); // Give the Player some health
};
Player::Player(){
	m_Speed=START_SPEED;
	m_Health=START_HEALTH;
	m_MaxHealth=START_HEALTH;
	m_Texture.loadFromFile("graphics/player.png");
	m_Sprite.setTexture(m_Texture);
	m_Sprite.setOrigin(25,25);
}
FloatRect Player::getPosition(){
	return m_Sprite.getGlobalBounds();
}
Sprite Player::getSprite(){
	return m_Sprite;
}
Vector2f Player::getCenter(){
	return m_Position;
}
float Player::getRotation(){
	return m_Sprite.getRotation();
}
void Player::spawn(IntRect arena, Vector2f resolution, int tileSize){
	m_Position.x=arena.width/2;// place the player in the middle of the arena
	m_Position.y=arena.height/2;
	m_Arena.left=arena.left;//Copy the details of the arena
	m_Arena.width=arena.width;
	m_Arena.top=arena.top;
	m_Arena.height=arena.height;
	m_TileSize=tileSize;//Remember how big the tiles are in this arena
	m_Resolution.x=resolution.x;//Store the resolution for future use
	m_Resolution.y=resolution.y;
}
void Player::moveRight(){
	m_RightPressed=true;
}
void Player::moveLeft(){
	m_LeftPressed=true;
}
void Player::moveUp(){
	m_UpPressed=true;
}
void Player::moveDown(){
	m_DownPressed=true;
}
void Player::stopRight(){
	m_RightPressed=false;
}
void Player::stopLeft(){
	m_LeftPressed=false;
}
void Player::stopUp(){
	m_UpPressed=false;
}
void Player::stopDown(){
	m_DownPressed=false;
}
void Player::update(float elapsedTime, Vector2i mousePosition){
	if(m_UpPressed){
		m_Position.y-=m_Speed*elapsedTime;
    }
    if(m_DownPressed){
		m_Position.y +=m_Speed*elapsedTime;
    }
	if(m_RightPressed){
		m_Position.x +=m_Speed*elapsedTime;
    }
	if(m_LeftPressed){
		m_Position.x-=m_Speed*elapsedTime;
    }
	m_Sprite.setPosition(m_Position);
	//Keep the Player in the arena
	if(m_Position.x>m_Arena.width - m_TileSize){
		m_Position.x=m_Arena.width - m_TileSize;
    }
	if(m_Position.x < m_Arena.left + m_TileSize){
		m_Position.x=m_Arena.left + m_TileSize;
    }
	if(m_Position.y > m_Arena.height - m_TileSize){
		m_Position.y=m_Arena.height - m_TileSize;
    }
	if(m_Position.y < m_Arena.top + m_TileSize){
		m_Position.y=m_Arena.top + m_TileSize;
    }
	float angle=(atan2(mousePosition.y - m_Resolution.y/2, mousePosition.x - m_Resolution.x/2)*180)/3.141;
	m_Sprite.setRotation(angle);	
}
bool Player::hit(Time timeHit){
	if(timeHit.asMilliseconds()-m_LastHit.asMilliseconds()>200){
		m_LastHit=timeHit;
		m_Health-=10;
	    return true;
    }
	else{
		return false;
	}
}
int Player::getHealth(){
	return m_Health;
}
void Player::upgradeSpeed(){//20% speed upgrade
	m_Speed+=(START_SPEED*0.2);
}
void Player::upgradeHealth(){	// 20% max health upgrade
	m_MaxHealth+=(START_HEALTH *0.2);
}
void Player::resetPlayerStats(){
	m_Speed=START_SPEED;
	m_Health=START_HEALTH;
	m_MaxHealth=START_HEALTH;
}

