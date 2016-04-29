#ifndef EnemyManager
#define EnemyManager

/** Recursive function which generates an x free coord for the enemy using a delta (ENEMY_DELTA_SPAWN)
* @param[in] x is used to return the x free cord when we have found it
* @param[in] game is used to check if other enemies seize the position (interval from x-ENEMY_DELTA_SPAWN to x+ENEMY_DELTA_SPAWN)
*
* The recursion ends when we find a free x coord so that the enemy hasn't others enemies around it in an interval
* with a radius of ENEMY_DELTA_SPAWN.
* The function uses rand() (we have set the random seed in ::InitGame) to create a random x coord.
*/
void GenerateFreeCoord(int &x, Game &game);

/**
* Initializes an enemy
* @param[in] game is used to add the enemy to the enemies' list
* @param[in] enemyImage is used to assign the enemy bitmap to the enemy that we have created
*
* The function initializes the enemy size, initial position (by finding a free x coord => see ::GenerateFreeCoord),
* its velocity, image and animation.
*/
void InitEnemy(Game &game, ALLEGRO_BITMAP *enemyImage);

/**
* Calls :UpdateAnimationFrame for each enemy in enemies' list
* @param[in] game is used to access to the enemies' list
*
*/
void UpdateEnemiesFrame(Game &game);

/**
* Draws enemies according to their positions
* @param[in] game is used to access to the enemies' list
*
*/
void DrawEnemies(const Game &game);

/**
* Moves enemies by updating their positions
* @param[in] game is used to access to the enemies' list
* @param[in] player is used to decrement player lifes if an enemy crossed the window
*
* If an enemy crosses the window we decrease player lifes.
* If player has 0 lifes left the game state is set to GameOver.
*/
void MoveEnemies(Game &game, Player &player);

/**
* Initializes an explosion
* @param[in] game is used to add the explosion to the explosions list
* @param[in] enemy is used as the base position to initialize the explosion
*
* The function initializes the explosion size, initial position, image and animation.
*/
void InitExplosion(Game &game, const Enemy &enemy);

/**
* Updates explosions animations
* @param[in] game is used to access to the explosions' list
*
* We destroy the explosion if the animation finishes (::UpdateAnimationFrame returns false)
*/
void UpdateExplosions(Game &game);

/**
* Draws explosions on the screen
* @param[in] game is used to access to the explosions' list
*
*/
void DrawExplosions(const Game &game);

#endif // Enemy

