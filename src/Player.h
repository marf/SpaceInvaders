#include "Data.h"

/** Initializes the player
* @param[in] player is used to initialize its variables
* The function initializes the player's size, initial position, its velocity, lifes, fire rate and animation.
*/
void InitPlayer(Player &player);

/** Moves the player
* @param[in] player is used to access and update its position and direction
* @param[in] keys is an array of bool used to move the player according to the keys we pressed
*
* The function also checks if we are at the borders of the window to avoid the player going outside the it.
*/
void MovePlayer(Player &player, const bool keys[]);

/** Kills the player
* @param[in] game is used to set the game state to GameOver
* @param[in] player is used to set player state to Death and to clear its bullets
*
*/
void KillPlayer(Game &game, Player &player);

/**
* Fires a bullet if we have finished reloading
* @param[in] player is used to check if we are ready to fire and to add a bullet
*
* We check if we are ready to fire by subracting the current time to the next time we will be ready to fire.
* If the result is > 0 it means that player.nextFire is in the past and the reloading process is finished.
* When we fire we set the player.nextFire time to the current time + PLAYER_FIRE_RATE
* which is the time we will spend for the reloading process.
*/
void Fire(Player &player);

/**
* Draws the player according to its direction
* @param[in] player is used to draw the player to the correct position and with the correct rotation
*
* We select the correct frame for the animation, than we draw the player according to its direction.
*/
void DrawPlayer(const Player &player);

/**
* Initializes a bullet
* @param[in] bullet is used to set bullet's size, initial position, velocity, image and animation
* @param[in] player is used as the base position to initialize the bullet
*
* The function initializes the bullet's size, initial position, velocity, image and animation.
*/
void InitBullet(Transform &bullet, Player &player);

/**
* Moves bullets by updating their positions
* @param[in] player is used to access to the bullets' list
*
* We destroy the bullet if it goes outside the window.
*/
void MoveBullets(Player &player);

/**
* Draws bullets according to their positions
* @param[in] player is used to access to the bullets' list
*/
void DrawBullets(const Player &player);

