#include<SFML/Graphics.hpp>
#include<sstream>
#include<iostream>
#include"player.cpp"
#include"zombie.cpp"
#include"bullet.cpp"
using namespace std;
using namespace sf;
int createBackground(VertexArray& rVA, IntRect arena);
Zombie* createHorde(int numZombies, IntRect arena);
int main(){
	enum class State{PAUSED, LEVELING_UP, GAME_OVER, PLAYING};
	State state=State::GAME_OVER;// Start with game over state
	// Get the screen Resolution and creata an SFML window
	Vector2f resolution;
	resolution.x=VideoMode::getDesktopMode().width;
	resolution.y=VideoMode::getDesktopMode().height;
	RenderWindow window(VideoMode(resolution.x,resolution.y),"ZOMBIE ARENA!!", Style::Fullscreen);
	View mainView(sf::FloatRect(0,0,resolution.x, resolution.y));//Create a SFML view for the main action
	Clock clock;
	Time gameTimeTotal;//How long has the PLAYING state been active
	Vector2f mouseWorldPosition;//Where is the Mouse in relation to the world coordinates
	Vector2i mouseScreenPosition;//Where is the Mouse in relation to the screen coordinates
    // Hide the mouse pointer and replace it with crosshair
	window.setMouseCursorVisible(true);
	Texture textureCrosshair;
	textureCrosshair.loadFromFile("graphics/crosshair.png");
	Sprite spriteCrosshair;
	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(25, 25);
	Player player;//Create an instance of the player class
	IntRect arena;// The boundaries of the Arena
	VertexArray background;  // Create the background
	Texture textureBackground;// Load the texture for our background vertex array
	textureBackground.loadFromFile("graphics/background_sheet.png");
    // Prepare for a horde of zombies
	int numZombies;
	int numZombiesAlive;
	Zombie* zombies = nullptr;
   // 100 bullets should do
	Bullet bullets[100];
	int currentBullet=0;
	int bulletsSpare= 24;
	int bulletsInClip=6;
	int clipSize=6;
	float fireRate=1;
	Time lastPressed;// When was the fire button last pressed?
    // About the game
	int score = 0;
	int hiScore = 0;
    // Create a view for the HUD--------------------------------------
	View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));
    // For the home/game over screen
	Sprite spriteGameOver;
	Texture textureGameOver; 
    textureGameOver.loadFromFile("graphics/background.png");
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setPosition(0, 0);
    // Create a sprite for the ammo icon
	Sprite spriteAmmoIcon;
	Texture textureAmmoIcon;
    textureAmmoIcon.loadFromFile("graphics/ammo_icon.png");
	spriteAmmoIcon.setTexture(textureAmmoIcon);
	spriteAmmoIcon.setPosition(20, 980);
    // Game Over
    Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");
	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(125);
	//gameOverText.setFillColor(Color::White);
	gameOverText.setPosition(250, 850);
	gameOverText.setString("Press Enter to play");
    // Levelling up
	Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(80);
//	levelUpText.setFillColor(Color::White);
	levelUpText.setPosition(150,250);
	std::stringstream levelUpStream;
	levelUpStream<<"1- Increased rate of fire" <<"\n2- Increased clip size(next reload)"<<
		"\n3- Increased max health"<<"\n4- Increased run speed";
	levelUpText.setString(levelUpStream.str());
    // Ammo
	Text ammoText;
	ammoText.setFont(font);
	ammoText.setCharacterSize(55);
	//ammoText.setFillColor(Color::White);
	ammoText.setPosition(200, 980);
    // Paused
	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(130);
	//pausedText.setFillColor(Color::White);
	pausedText.setPosition(400, 400);
	pausedText.setString("Press Enter \nto continue");
	// Score
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(55);
	//scoreText.setFillColor(Color::White);
	scoreText.setPosition(20, 0);
	// Hi Score
	Text hiScoreText;
	hiScoreText.setFont(font);
	hiScoreText.setCharacterSize(55);
	//hiScoreText.setFillColor(Color::White);
	hiScoreText.setPosition(1400, 0);
	std::stringstream s;
	s << "Hi Score:" << hiScore;
	hiScoreText.setString(s.str());
    // Zombies remaining
	Text zombiesRemainingText;
	zombiesRemainingText.setFont(font);
	zombiesRemainingText.setCharacterSize(55);
	//zombiesRemainingText.setFillColor(Color::White);
	zombiesRemainingText.setPosition(1500, 980);
	zombiesRemainingText.setString("Zombies: 100");

	// Wave number
	int wave = 0;
	Text waveNumberText;
	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(55);
	//waveNumberText.setFillColor(Color::White);
	waveNumberText.setPosition(1250, 980);
	waveNumberText.setString("Wave: 0");

	// Health bar
	RectangleShape healthBar;
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(450, 980);
	while(window.isOpen()){
		/*--------------------Handle Input------------------------------------------------------------*/
		// Handle Events by Polling----------------------------------------------------------------
		Event event;
		while(window.pollEvent(event)){
			if(event.type==Event::KeyPressed){
				// Pause a game while Playing
				if(event.key.code==Keyboard::Return && state==State::PLAYING){
					state=State::PAUSED;
				}
            	// Restart while paused
				else if(event.key.code==Keyboard::Return && state==State::PAUSED){
					state=State::PLAYING;
					clock.restart();
				}
 				// Start a new Game while in GAME_OVER state
				else if(event.key.code==Keyboard::Return && state==State::GAME_OVER){
					state=State::LEVELING_UP;
					wave = 0;
					score = 0;
					// Prepare the gun and ammo for next game
					currentBullet = 0;
					bulletsSpare = 24;
					bulletsInClip = 6;
					clipSize = 6;
					fireRate = 1;
					// Reset the player's stats
					player.resetPlayerStats();
				}	
                if(state==State::PLAYING){
					if (event.key.code == Keyboard::R){   // Reloading
						if (bulletsSpare >= clipSize){// Plenty of bullets. Reload.
							bulletsInClip = clipSize;
							bulletsSpare -= clipSize;
						}
						else if (bulletsSpare > 0){// Only few bullets left
							bulletsInClip = bulletsSpare;
							bulletsSpare = 0;
						}
						else
						{
							// sound play: reload
						}
					}
				}// End of if state::PLAYING	
			}// end if KeyPressed
			if(event.type==Event::Closed){
				window.close();
            }
		}// End event polling-------------------------------------------------------------------------
		/*----------- Handle the Player quitting---------------------------------------------------*/
		if(Keyboard::isKeyPressed(Keyboard::Escape)){
			window.close();
		}
        /*---------- Handle WASD while Playing -------------------------------------------------*/
		if(state==State::PLAYING){// Handle the pressing and releasing of WASD keys
			if(Keyboard::isKeyPressed(Keyboard::W)){
				player.moveUp();
			}
			else{
				player.stopUp();
			}
			if(Keyboard::isKeyPressed(Keyboard::S)){
				player.moveDown();
			}
			else{
				player.stopDown();
 			}
			if(Keyboard::isKeyPressed(Keyboard::A)){
				player.moveLeft();
			}
			else{
				player.stopLeft();
			}
			if(Keyboard::isKeyPressed(Keyboard::D)){
				player.moveRight();
			}
			else{
				player.stopRight();
			}
			// Fire a bullet
			if(Mouse::isButtonPressed(Mouse::Left)){
				if(gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate && bulletsInClip > 0){
					// Pass the centre of the player and the centre of the cross-hair to the shoot function
					bullets[currentBullet].shoot(player.getCenter().x, player.getCenter().y,
						mouseWorldPosition.x, mouseWorldPosition.y);
					currentBullet++;
					if (currentBullet > 99){
						currentBullet = 0;
					}
					lastPressed = gameTimeTotal;
					bulletsInClip--;
				}
			}// End fire a bullet
    
 		}// End if, WASD while playing
		/* -------------------Handle the LEVELING up state--------------------------------------*/
		if(state==State::LEVELING_UP){
			if(event.key.code==Keyboard::Num1){
				// Increase fire rate
				fireRate++;
				state=State::PLAYING;
			}
			if(event.key.code==Keyboard::Num2){
				// Increase clip size
				clipSize += clipSize;
				state=State::PLAYING;
			}
			if(event.key.code==Keyboard::Num3){
				// Increase health
				player.upgradeHealth();
				state=State::PLAYING;
			}
			if(event.key.code==Keyboard::Num4){
				// Increase speed
				player.upgradeSpeed();
				state=State::PLAYING;
			}
			if(event.key.code==Keyboard::Num5){
				state=State::PLAYING;
			}
			if(event.key.code==Keyboard::Num6){
				state=State::PLAYING;
			}
			if(state==State::PLAYING){
				wave++; // Increase the wave number
				arena.width = 500 * wave;//Prepare the level
				arena.height = 500 * wave;
				arena.left=0;
				arena.top=0;
                // Pass the vertex array by reference to the createBackground function
				int tileSize = createBackground(background, arena);
				// span the player in the middle of the arena
				player.spawn(arena, resolution, tileSize);
				numZombies = 3 * wave;// Create a horde of zombies
				// Delete the previously allocated memory (if it exists)
				delete[] zombies;
				zombies = createHorde(numZombies, arena);
				numZombiesAlive = numZombies;
				clock.restart();// Reset the clock
			}
		}// End LEVELING up
		/*---------------------------UPDATE THE FRAME------------------------------------*/
        if(state==State::PLAYING){
			Time dt=clock.restart(); // update delta time
			gameTimeTotal +=dt;
			float dtAsSeconds=dt.asSeconds();
			mouseScreenPosition=Mouse::getPosition();
            mouseWorldPosition=window.mapPixelToCoords(Mouse::getPosition(),mainView);
            spriteCrosshair.setPosition(mouseWorldPosition);
			player.update(dtAsSeconds,Mouse::getPosition());  //Update the player
			Vector2f playerPosition(player.getCenter()); // Vector2f constructor;playerPosition- is an Object
		    mainView.setCenter(player.getCenter()); //Make the view center around the player
            // Loop through each Zombie and update them
			for (int i = 0; i < numZombies; i++){
				if (zombies[i].isAlive()){
					zombies[i].update(dt.asSeconds(), playerPosition);
				}
            }
            // Update any bullets that are in-flight
			for (int i = 0; i < 100; i++){
				if (bullets[i].isInFlight()){
					bullets[i].update(dtAsSeconds);
				}
			}
         /*======== COLLISION DETECTION=================*/
         //------------Have any zombies touched the player			
			for (int i = 0; i < numZombies; i++){
				if (player.getPosition().intersects(zombies[i].getPosition()) && zombies[i].isAlive()){
					if (player.hit(gameTimeTotal)){
						// More here later//hit.play();
					}
					if (player.getHealth() <= 0){
						state = State::GAME_OVER;					
					}
				}
			}// End player touched
           /*----------------------Have any zombies been shot?-------------------------------*/
			for (int i = 0; i < 100; i++){
				for (int j = 0; j < numZombies; j++){
					if (bullets[i].isInFlight() && zombies[j].isAlive()){
						if (bullets[i].getPosition().intersects(zombies[j].getPosition())){
							bullets[i].stop();// Stop the bullet
							if (zombies[j].hit()) {// Register the hit and see if it was a kill
								score += 10;// Not just a hit but a kill too
								if (score >= hiScore){
									hiScore = score;
								}
								numZombiesAlive--;
								if (numZombiesAlive == 0) {// When all the zombies are dead (again)
									state = State::LEVELING_UP;
								}
							}	
					     }
					}
				}
			}// End zombie being shot
			// ---------------------size up the health bar-----------------
			healthBar.setSize(Vector2f(player.getHealth() * 3, 70));
           //-----------------------Update HUD---------------------------
                // Update the score text
                std::stringstream ssScore;
				ssScore << "Score:" << score;
				scoreText.setString(ssScore.str());

				// Update the high score text
                std::stringstream ssHiScore;
				ssHiScore << "Hi Score:" << hiScore;
				hiScoreText.setString(ssHiScore.str());
                // Update the ammo text
                std::stringstream ssAmmo;
				ssAmmo << bulletsInClip << "/" << bulletsSpare;
				ammoText.setString(ssAmmo.str());
				// Update the wave
                std::stringstream ssWave;
				ssWave << "Wave:" << wave;
				waveNumberText.setString(ssWave.str());

				// Update the zombies
                std::stringstream ssZombiesAlive;
				ssZombiesAlive << "Zombies:" << numZombiesAlive;
				zombiesRemainingText.setString(ssZombiesAlive.str());

		}// End updating scene
		/*---------------------------DRAW the SCENE-----------------------------------------*/
		if(state==State::PLAYING){
			window.clear();
			window.setView(mainView);
			window.draw(background, &textureBackground);// Draw the background
			// Draw the zombies
			for (int i = 0; i < numZombies; i++){
				window.draw(zombies[i].getSprite());	
			}
            // Drawing the bullets in each frame
			for (int i = 0; i < 100; i++){
				if (bullets[i].isInFlight()){
					window.draw(bullets[i].getShape());
				}				
			}
			// Draw the player
			window.draw(player.getSprite());	
           //Draw the crosshair
			window.draw(spriteCrosshair);	
			// Switch to the HUD view
			window.setView(hudView);
			// Draw all the HUD elements
            window.draw(spriteAmmoIcon);
			window.draw(ammoText);
			window.draw(scoreText);
			window.draw(hiScoreText);
			window.draw(healthBar);
			window.draw(waveNumberText);
			window.draw(zombiesRemainingText);
		}
    	if(state==State::LEVELING_UP){
           window.draw(spriteGameOver);
		   window.draw(levelUpText);
		}
		if(state==State::PAUSED){
			window.draw(pausedText);
		}
		if(state==State::GAME_OVER){
            window.draw(spriteGameOver);
            window.draw(gameOverText);
			window.draw(scoreText);
			window.draw(hiScoreText);
		}
		window.display();
	}// end of main game loop
	return 0;
}
/*---------------------------------------------------------------------------------------------------*/
int createBackground(VertexArray& rVA, IntRect arena)
{
	const int TILE_SIZE = 50;
	const int TILE_TYPES = 3;
	const int VERTS_IN_QUAD = 4;
	int worldWidth = arena.width / TILE_SIZE;
	int worldHeight = arena.height / TILE_SIZE;
	// What type of primitive are we using?
	rVA.setPrimitiveType(Quads);
	// Set the size of the vertex array
	rVA.resize(worldWidth * worldHeight * VERTS_IN_QUAD);
	// Start at the beginning of the vertex array
	int currentVertex = 0;
	for (int w = 0; w < worldWidth; w++)
	{
		for (int h = 0; h < worldHeight; h++)
		{
			// Position each vertex in the current quad
			rVA[currentVertex + 0].position = Vector2f(w * TILE_SIZE, h * TILE_SIZE);
			rVA[currentVertex + 1].position = Vector2f((w * TILE_SIZE) + TILE_SIZE, h * TILE_SIZE);
			rVA[currentVertex + 2].position = Vector2f((w * TILE_SIZE) + TILE_SIZE, (h * TILE_SIZE) + TILE_SIZE);
			rVA[currentVertex + 3].position = Vector2f((w * TILE_SIZE), (h * TILE_SIZE) + TILE_SIZE);

			// Define the position in the Texture to draw for current quad:Either mud, stone, grass or wall
			if (h == 0 || h == worldHeight - 1 || w == 0 || w == worldWidth - 1)
			{
				// Use the wall texture
				rVA[currentVertex + 0].texCoords = Vector2f(0, 0 + TILE_TYPES * TILE_SIZE);
				rVA[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, 0 + TILE_TYPES * TILE_SIZE);
				rVA[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE + TILE_TYPES * TILE_SIZE);
				rVA[currentVertex + 3].texCoords = Vector2f(0, TILE_SIZE + TILE_TYPES * TILE_SIZE);
			}
			else
			{
				// Use a random floor texture
				srand((int)time(0) + h * w - h);
				int mOrG = (rand() % TILE_TYPES);
				int verticalOffset = mOrG * TILE_SIZE;
				rVA[currentVertex + 0].texCoords = Vector2f(0, 0 + verticalOffset);
				rVA[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, 0 + verticalOffset);
				rVA[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE + verticalOffset);
				rVA[currentVertex + 3].texCoords = Vector2f(0, TILE_SIZE + verticalOffset);

			}
			// Position ready for the next for vertices
			currentVertex = currentVertex + VERTS_IN_QUAD;
		}
	}
	return TILE_SIZE;
}
/*------------------Zombie Horde Function definition-------------*/
Zombie* createHorde(int numZombies, IntRect arena)
{
	Zombie* zombies = new Zombie[numZombies];
	int maxY = arena.height - 20;
	int minY = arena.top + 20;
	int maxX = arena.width - 20;
	int minX = arena.left + 20;
	for (int i = 0; i < numZombies; i++){
		srand((int)time(0) * i);// Which side should the zombie spawn
		int side = (rand() % 4);
		float x, y;
		switch (side){
		case 0:// left
			x = minX;
			y = rand() %(maxY-minY) + minY;
			break;
		case 1:// right
			x = maxX;
			y = rand() %(maxY-minY) + minY;
			break;
		case 2:// top
			x = rand() %(maxX-minX) + minX;
			y = minY;
			break;

		case 3:
			// bottom
			x = rand() %(maxX-minX) + minX;
			y = maxY;
			break;
		} // end of switch
		srand((int)time(0) * i * 2); // Zombie type: Bloater, crawler or runner
		int type = (rand() % 3);
		zombies[i].spawn(x, y, type, i);// Spawn the new zombie into the array
	} // enf of for
	return zombies;
}
