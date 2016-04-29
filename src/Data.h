#ifndef Data
#define Data

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <vector>

using namespace std;

/** Width of the screen */
const int SCREEN_WIDTH = 800;
/** Height of the screen */
const int SCREEN_HEIGHT = 600;

/** File path and name to save records */
const char RECORD_FILE_NAME[] = "data/record.txt";

/** FPS, increase it to increase the velocity of the game */
const int FPS = 60;

/** Animations' velocity (used for player, enemies and powerup) */
const int FRAMES_TIME = 15;

/* PLAYER CONFIGURATION */

/** Player single frame width */
const int PLAYER_SIZE_W = 29;

/** Player single frame height */
const int PLAYER_SIZE_H = 64;

/** Player initial velocity */
const int PLAYER_INIT_VEL = 7;

/** Player reloading time */
const double PLAYER_FIRE_RATE = 0.5;

/** Player frames for animation */
const int PLAYER_FRAMES = 4;

/** Player lifes */
const int PLAYER_LIFES = 3;


/* ENEMY CONFIGURATION */

/** Enemy spawn frequency */
const float ENEMY_SPAWN_TIME = 2;

/** Enemy interval radius to mantain distance between enemies */
const int ENEMY_DELTA_SPAWN = 20;

/** Enemy frames for animation */
const int ENEMY_FRAMES = 6;

/** Enemies images */
const int ENEMY_IMAGES = 4;

/** Enemy single frame width */
const int ENEMY_SIZE_W = 37;

/** Enemy single frame height */
const int ENEMY_SIZE_H = 35;

/** Enemy initial velocity */
const int ENEMY_INIT_VEL = 2;

/* BULLET CONFIGURATION */

/** Bullet width */
const int BULLET_SIZE_W = 9;

/** Bullet height */
const int BULLET_SIZE_H = 9;

/** Bullet initial velocity */
const int BULLET_INIT_VEL = 2;

/* EXPLOSION CONFIGURATION */

/** Explosion duration calculated using the formula 1.0/EXPLOSION_TIME */
const int EXPLOSION_TIME = 20;
/** Explosion frames for animation */
const int EXPLOSION_FRAMES = 8;
/** Explosion single frame width */
const int EXPLOSION_SIZE_W = 32;
/** Explosion single frame height */
const int EXPLOSION_SIZE_H = 32;

/* BACKGROUNDS CONFIGURATION */

/** Background image height (need to be equal or bigger than the SCREEN_HEIGHT */
const int BG_IMAGE_HEIGHT = 600;

/** Number of different backgrounds (used for parallax effect) */
const int BG_NUM = 2;

/** Initial velocity of the main background (the others will be increased in velocity using this velocity as a reference
* to achieve the parallax effect) */
const int BACKGROUND_INIT_VEL = 3;

/* SCOREBOARD CONFIGURATION */

/** Scoreboard font path and name */
const char SCOREBOARD_FONT_NAME[] = "fonts/ScoreboardFont.ttf";

/** Scoreboard font size */
const int SCOREBOARD_FONT_SIZE = 20;

/** Space between life images in the top right corner */
const int LIFES_IMAGE_SPACE = 30;

/** Game Over image width */
const int GAME_OVER_SIZE_W = 250;

/** Game Over image height */
const int GAME_OVER_SIZE_H = 60;

/* POWERUP CONFIGURATION */

/** PowerUp single frame width */
const int POWERUP_SIZE_W = 18;
/** PowerUp single rame height */
const int POWERUP_SIZE_H = 18;
/** Plasma barrier height */
const int PLASMA_BARRIER_HEIGHT = 50;
/** After how many enemies killed we spawn the power up? */
const int POWERUP_FREQUENCY = 4;
/** Initial powerup velocity */
const int POWERUP_INIT_VEL = 1;
/** PowerUp pickup time */
const int POWERUP_TIME = 3;
/** PowerUp effect time */
const int POWERUP_EFFECT_TIME = 10;
/** PowerUp franes for animation */
const int POWERUP_FRAMES = 15;

/* DATA STRUCTURES */

/** Directions used by the player */
enum Directions { UP, DOWN, LEFT, RIGHT, NONE };

/** Generic transform structure used to store positions, velocity and current frame (for animations) of an object */
struct Transform
{
    /** x and y coords used to find the position of an object in a 2D space */
    int x, y;
    /** Velocity of the object */
    int vel;
    /** Current frame of animation of the object (if it has animations) */
    int currentFrame;
    /** Width of the transform */
    int width;
    /** Height of the transform */
    int height;

    /** Image of the transform */
    ALLEGRO_BITMAP *image;
};

/** Enemy data structure */
struct Enemy
{
    /** Enemy transform */
    Transform transform;
    /** Enemy leaves a power up after death */
    bool hasPowerUp;

};

/** Player data structure */
struct Player
{
    /** Player transform */
    Transform transform;

    /** Player direction */
    Directions dir;

    /** Player lifes */
    int lifes;

    /** Is player alive? */
    bool isAlive;

    /** Is player firing? */
    bool isFiring;

    /** Next time that the player can fire */
    time_t nextFire;

    /** List of the bullets fired by the player */
    vector<Transform> bullets;

    /** Player's bullet image */
    ALLEGRO_BITMAP *bulletImage;
};

/** Game data structure */
struct Game
{
    /** List of the enemies spawned */
    vector<Enemy> enemies;

    /** List of the explosions */
    vector<Transform> explosions;

    /** Points of the current game session */
    int points;

    /** Best record of the player */
    int record;

    /** Number of enemies spawned */
    int enemiesCount;

    int enemiesKilledCount;

    bool increaseVel;

    /** PowerUp transform */
    Transform powerup;

    /** Barrier transform */
    Transform barrier;

    /** Is powerup spawned? */
    bool isPowerUpSpawned;

    /** Is powerup active ? */
    bool isPowerUpActive;

    /** Static array of backgrounds (each type of background has two background structures) */
    Transform backgrounds[2*BG_NUM]; // fixed size for backgrounds

    /** Is the game over? */
    bool isOver;

    /** Is the game paused? */
    bool isPaused;

    /** Explosion image */
    ALLEGRO_BITMAP *explosionImage;
};






#endif // Data

