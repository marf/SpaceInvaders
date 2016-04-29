#ifndef GameManager
#define GameManager

#include "Data.h"

/**
* Initializes all the allegro's components including keybord, a display, a queue, four timers, a font and all the bitmaps
* @param[in] game is the current game session which has all the necessary variables for the gameplay
* @param[in] player is the object that we can move and play with
*
* If something couldn't be initialize the game stops with an error that help the developer in finding the problem.
* This function uses assertions to handle errors.
*/
void InitGame(Game &game, Player &player);


/**
* Initializes the backgrounds for the parallax effect
* @param[in] game is used to access to the backgrounds
*
* Every background is a pair and the main one of them is the first object of the pair.
* The main background of each pair is drawn in front of the screen,
* while the other is drawn outside the screen game.backgrounds[i].y = -BG_IMAGE_HEIGHT.
* Every background has a velocity, the first pair is slower than the others pairs in order to achieve a parallax effect.
*/
void InitBackgrounds(Game &game);

/**
* Handle all the input and timers events of the game
* @param[in] KeyCode is the code of the keybord button we have released or pressed
* @param[in] game is used to init/update enemies and update backgrounds, it is also used to update enemies and explosions' animations
* @param[in] player is used to move the player, detect collisions and update player's animations
* @param[in] keys is an array of bool that we use to handle movement
* @param[in] Exit is used to stop and quit the game
* @param[in] Draw is used to draw all the bitmaps and fonts on the screen when the GameTimer event is triggered
*
* We use GameTimer to update player, bullets, enemies and backgrounds positions.
* EnemyTimer is used to spawn enemies.
* FrameTimer is used to update animations.
* ExplosionTimer is used to update explosions.
* After all the events are handled we set Draw to true in order to re-draw everything using ::Render.
*/
void EventManager(Player &player, Game &game, bool keys[], bool &Exit, bool &Draw);

/**
* Renders every object on the screen by using the double buffer technique
* @param[in] player is used to draw the player and its bullets to the correct position
* @param[in] game is used to draw the background, enemies, explosions and the scoreboard
* @param[in] Draw is used to check if we need to re-draw everything (we have to draw after ::EventManager is called)
*
* After the render finishes Draw is set to false in order to wait for all the updates of ::EventManager to do another draw.
*/
void Render(Player &player, Game &game, bool &Draw);

/**
* Handles the keyup event of a keybord device
* @param[in] KeyCode is the code of the keybord button we have released
* @param[in] game is used to pause the game or to clear the enemies if we have restarted the game
* @param[in] player is used to save the direction of the player to NONE if we release a keybord's button which triggers a moving action
* @param[in] keys is an array of bool that we use to set to false the pressed state of the keys used for the moving actions
* @param[in] Exit is used to stop and quit the game
*/
void HandleKeyUp(int KeyCode, Game &game, Player &player, bool keys[], bool &Exit);

/**
* Handles the keydown event of a keybord device
* @param[in] KeyCode is the code of the keybord button we have pressed
* @param[in] keys is an array of bool that we use to set to true a specific move button in order to move the player
*/
void HandleKeyDown(int KeyCode, bool keys[]);

/**
* Draw the scoreboard including player's points, the maximum record (saved in a file) and player's lifes
* @param[in] game is used to access to the points and the record
* @param[in] player is used to access to player's lifes
*
* We use a stringstream to easily build a string and than we convert it to a *char.
* If the game is over or paused we draw a specific bitmap in front of the screen.
* Record is only displayed if we have played the game at least two times.
*/
void DrawScoreBoard(Game &game, Player &player);

/**
* Moves the backgrounds from top to bottom, than it calls ::CheckBgLimit to check if background y coord is valid
* @param[in] game is used to access to the backgrounds' list
*
*/
void UpdateBackgrounds(Game &game);

/**
* Draws backgrounds according to their positions
* @param[in] game is used to access to the backgrounds' list
*
*/
void DrawBackgrounds(Game &game);

/**
* Checks if the background is at the bottom of the screen,
* in this case the function moves it outside the top of the screen
* @param[in] bg a transform that refresents the background that we want to check
*
* This effect is important because with a pair of backgrounds we create a cycle
* where the blank space that one background leaves is filled by the other background of the pair.
*/
void CheckBgLimit(Transform &bg);

/**
* Detects collisions between enemies and player's bullets, player and enemies,
* player and powerup and between enemies and plasma barrier by calling ::DetectCollision
* @param[in] player is our current player that we use to detect collisions
* @param[in] game is used to access to enemies' positions to detect collisions
*/
void DetectCollisions(Player &player, Game &game);

/**
* Detects different collisions.
* The collisions are between an enemy with index enemyIndex and a player's bullet with index bulletIndex
* (if bulletIndex is different from -1) or between the enemy and the plasma barrier (if it's enabled).
* If we have a powerup also it checks collisions between the powerup and the player.
* @param[in] player is our current player that we use to detect collisions
* @param[in] game is used to access to the enemy with index enemyIndex
* @param[in] enemyIndex is the index of the enemy which we selected for collision
* @param[in] bulleIndex is the index of the player's bullet which we selected for collision (optional, used only while checking collisions if we have bullets)
*
* If there's a collision we call ::InitExplosion to initialize an explosion in the position of the collision.
* If the collision is between the enemy and player's bullet we delete the enemy and the bullet from their lists and add a point to the player.
* If the collision is between the enemy and the plasma barrier we delete the enemy but we don't spawn another powerup (if that enemy has one),
* also we add a point to the player.
* If the collision is between the enemy and the player we kill the player and set the game state to GameOver.
* If the collision is between the powerup and the player we activate the powerup effect.
*/
void DetectCollision(Player &player, Game &game, int enemyIndex, int bulletIndex = -1);

/**
* Initializes (visible is true) or destroys (visible is false) a power up at the given x and y and with the specified velocity
* @param[in] game is used to update powerup transform, including position, velocity and visibility
* @param[in] x is the x coord where we have to place the powerup
* @param[in] y is the y coord where we have to place the powerup
* @param[in] velocity is the velocity we have to assign to the powerup
* @param[in] visible is a bool which indicates if we have to initialize (visible is true) or destroy (visible is false) the powerup
*/
void UpdatePowerUp(Game &game, int x, int y, int velocity, bool visible);

/**
* Enables or disables the powerup effect
* @param[in] game is used to access to the powerup effect state
* @param[in] activated is used to enable/disable the powerup's effect
*
* If the powerup is disabled this function removes it
*/
void UpdatePowerUpEffect(Game &game, bool activated);

/**
* Checks if a previous record exists and returns it
* @return Returns an int that represent the record if it exist, if it doesn't exist the function returns 0
*
* Checks if a previous record exist by searching it in a file, if it doesn't exist returns 0
*/
int RecordExist();

/**
* Saves the new record in a file
* @param[in] game is used in order to access to the game's points which are a new record if this function is called
*
* The function creates or updates the file which stores the record in order to save the new highscore
*/
void WriteRecord(const Game &game);

/**
* Saves a new record if we have made one (by calling ::WriteRecord) than destroys everything and quits the game
* @param[in] player is used to clear all the player's bullets
* @param[in] game is used in order to access to the game's points to find if we have made a new record and to clear all the enemies
*
* This function uses assertions to handle errors.
*/
void QuitGame(Player &player, Game &game);

#endif // GameManager

