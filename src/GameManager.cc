#include <cstdio>
#include <ctime>
#include <sstream>
#include <assert.h>

#include "GameManager.h"
#include "Data.h"
#include "Player.h"
#include "Enemy.h"
#include "Engine.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <fstream>

using namespace std;

/* Global Variables */

/** Display variable necessary for allegro's functions */
ALLEGRO_DISPLAY     *Display = NULL;
/** Event queue in which we store all our allegro's events */
ALLEGRO_EVENT_QUEUE *Queue = NULL;
/** Timer used to update player, bullets, enemies and backgrounds positions */
ALLEGRO_TIMER       *GameTimer = NULL;
/** Timer used to spawn enemies */
ALLEGRO_TIMER       *EnemyTimer = NULL;
/** Timer used to update animations */
ALLEGRO_TIMER       *FrameTimer = NULL;
/** Timer used to update explosions' animations */
ALLEGRO_TIMER       *ExplosionTimer = NULL;
/** Timer used to destroy the current power up (if spawned) after a time given by  POWERUP_TIME */
ALLEGRO_TIMER       *PowerUpTimer = NULL;
/** Timer used to end the powerup effect after a time given by POWERUP_EFFECT_TIME */
ALLEGRO_TIMER       *PowerUpEffectTimer = NULL;
/** Font used to draw the scoreboard */
ALLEGRO_FONT        *Font = NULL;
/** Images used for enemies (we have enemies with differents colors) */
ALLEGRO_BITMAP      *EnemyImage[ENEMY_IMAGES];
/** Image used for the main background */
ALLEGRO_BITMAP      *BackGroundImage = NULL;
/** Image used for the secondary background (to achieve a parallax effect) */
ALLEGRO_BITMAP      *MediumGroundImage = NULL;
/** Image used for the GameOver state */
ALLEGRO_BITMAP      *GameOverImage = NULL;
/** Image used for the GamePaused state */
ALLEGRO_BITMAP      *GamePausedImage = NULL;
/** Image used for the player's lifes */
ALLEGRO_BITMAP      *LifesImage = NULL;
/** Image used for the powerup */
ALLEGRO_BITMAP      *PowerUpImage = NULL;
/** Image used for the plasma barrier */
ALLEGRO_BITMAP      *PlasmaBarrierImage = NULL;

/* --------------------- */

void InitGame(Game &game, Player &player)
{
    // Set random seed for generating enemy x coord.
    srand(time(0));

    assert(al_init());

	assert(al_install_keyboard());

    Display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);

    assert(Display);

	Queue = al_create_event_queue();

	assert(Queue);

	GameTimer = al_create_timer(1.0/FPS);

    assert(GameTimer);

    EnemyTimer = al_create_timer(ENEMY_SPAWN_TIME);

    assert(EnemyTimer);

    FrameTimer = al_create_timer(1.0/FRAMES_TIME);

    assert(FrameTimer);

    ExplosionTimer = al_create_timer(1.0/EXPLOSION_TIME);

    assert(ExplosionTimer);

    PowerUpTimer = al_create_timer(POWERUP_TIME);

    assert(PowerUpTimer);

    PowerUpEffectTimer = al_create_timer(POWERUP_EFFECT_TIME);

    assert(PowerUpEffectTimer);

    assert(al_init_image_addon());

    BackGroundImage = al_load_bitmap("media/starBG.png");

	assert(BackGroundImage);

    MediumGroundImage = al_load_bitmap("media/starFG.png");

	assert(MediumGroundImage);

    player.transform.image = al_load_bitmap("media/player.png");

	assert(player.transform.image);

    char EnemyImageName[] = "media/enemy0.png";

    for(int i = 0; i < ENEMY_IMAGES; i++)
    {
        EnemyImageName[11] = static_cast<char>(i+'0');
        EnemyImage[i] = al_load_bitmap(EnemyImageName);

        assert(EnemyImage[i]);
    }


    player.bulletImage = al_load_bitmap("media/laser.png");

	assert(player.bulletImage);

    game.explosionImage = al_load_bitmap("media/explosion.png");

    assert(game.explosionImage);

    GameOverImage = al_load_bitmap("media/gameover.png");

    assert(GameOverImage);

    GamePausedImage = al_load_bitmap("media/gamepaused.png");

    assert(GamePausedImage);

    LifesImage = al_load_bitmap("media/life.png");

    assert(LifesImage);

    PowerUpImage = al_load_bitmap("media/powerup.png");

    assert(PowerUpImage);

    PlasmaBarrierImage = al_load_bitmap("media/plasmabarrier.png");

    assert(PlasmaBarrierImage);

    al_init_font_addon();
    al_init_ttf_addon();

    Font = al_load_ttf_font(SCOREBOARD_FONT_NAME, SCOREBOARD_FONT_SIZE, 0);

    assert(Font);

	al_register_event_source(Queue, al_get_display_event_source(Display));
	al_register_event_source(Queue, al_get_timer_event_source(GameTimer));
	al_register_event_source(Queue, al_get_timer_event_source(EnemyTimer));
	al_register_event_source(Queue, al_get_timer_event_source(FrameTimer));
	al_register_event_source(Queue, al_get_timer_event_source(ExplosionTimer));
    al_register_event_source(Queue, al_get_timer_event_source(PowerUpTimer));
        al_register_event_source(Queue, al_get_timer_event_source(PowerUpEffectTimer));
	al_register_event_source(Queue, al_get_keyboard_event_source());

	al_start_timer(GameTimer);
	al_start_timer(EnemyTimer);
	al_start_timer(FrameTimer);
	al_start_timer(ExplosionTimer);

	game.isOver = false;
	game.isPaused = false;
	game.isPowerUpSpawned = false;
	game.isPowerUpActive = false;
	game.points = 0;
	game.record = RecordExist();
	game.enemiesCount = 0;
	game.enemiesKilledCount = 0;
	game.increaseVel = false;

    game.barrier.width = SCREEN_WIDTH;
    game.barrier.height = PLASMA_BARRIER_HEIGHT;
    game.barrier.x = 0;
    game.barrier.y = SCREEN_HEIGHT - game.barrier.height;
    game.barrier.currentFrame = -1;
    game.barrier.image = PlasmaBarrierImage;

    game.powerup.width = POWERUP_SIZE_W;
    game.powerup.height = POWERUP_SIZE_H;
    game.powerup.x = 0;
    game.powerup.y = 0;
    game.powerup.currentFrame = 0;
    game.powerup.image = PowerUpImage;

    InitBackgrounds(game);

    #ifdef DEBUG
        fprintf(stderr, "GAME STARTED! - InitGame\n");
    #endif // DEBUG


}

void InitBackgrounds(Game &game)
{
    for(int i = 0; i < 2*BG_NUM; i++)
    {
        game.backgrounds[i].x = 0;

        if(i%2 == 0) // main background for each background pair
        {
            game.backgrounds[i].y = 0;
        }
        else
        {
            game.backgrounds[i].y = -BG_IMAGE_HEIGHT;
        }


        if(i < 2) // First pair
        {
            game.backgrounds[i].vel = BACKGROUND_INIT_VEL;
            game.backgrounds[i].image = BackGroundImage;
        }
        else
        {
            game.backgrounds[i].vel = 1.5*BACKGROUND_INIT_VEL;
            game.backgrounds[i].image = MediumGroundImage;
        }

    }

    #ifdef DEBUG
        fprintf(stderr, "BACKGROUNDS INITIALIZED - InitBackgrounds\n");
    #endif // DEBUG

}

void EventManager(Player &player, Game &game, bool keys[], bool &Exit, bool &Draw)
{
    	ALLEGRO_EVENT ev;
		al_wait_for_event(Queue, &ev);

		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			Exit = true;

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
			HandleKeyDown(ev.keyboard.keycode, keys);

		if (ev.type == ALLEGRO_EVENT_KEY_UP)
			HandleKeyUp(ev.keyboard.keycode, game, player, keys, Exit);

        if(!game.isPaused)
        {

            if (ev.type == ALLEGRO_EVENT_TIMER)
            {
                if(ev.timer.source == GameTimer)
                {
                    if(player.isAlive)
                    {
                        MovePlayer(player, keys);
                        MoveBullets(player);
                        UpdateBackgrounds(game);

                        if(game.isPowerUpSpawned)
                            MoveTransform(game.powerup, POWERUP_INIT_VEL, DOWN);
                    }

                    MoveEnemies(game, player);
                    DetectCollisions(player, game);

                }

                else if(ev.timer.source == EnemyTimer)
                {
                    InitEnemy(game, EnemyImage[rand()%ENEMY_IMAGES]);
                }

                else if(ev.timer.source == FrameTimer)
                {
                    UpdateAnimationFrame(player.transform, PLAYER_FRAMES, true);
                    if(game.isPowerUpSpawned)
                        UpdateAnimationFrame(game.powerup, POWERUP_FRAMES, true);
                    UpdateEnemiesFrame(game);
                }
                else if(ev.timer.source == ExplosionTimer)
                {
                    UpdateExplosions(game);
                }
                else if(ev.timer.source == PowerUpTimer)
                {
                    if(game.isPowerUpSpawned)
                        UpdatePowerUp(game, 0, 0, 0, false);
                }
                else if(ev.timer.source == PowerUpEffectTimer)
                {
                    if(game.isPowerUpActive)
                        UpdatePowerUpEffect(game, false);
                }

            }

		}

        Draw = true;


}

void Render(Player &player, Game &game, bool &Draw)
{

    if (Draw && al_event_queue_is_empty(Queue))
		{
			Draw = false;
			al_clear_to_color(al_map_rgb(0, 0, 0));

            DrawBackgrounds(game);

            if(player.isAlive)
            {
                DrawPlayer(player);
                DrawBullets(player);
            }
			DrawEnemies(game);
			DrawExplosions(game);

			if(game.isPowerUpSpawned)
                DrawSprite(game.powerup);

            if(game.isPowerUpActive)
            {
                DrawSprite(game.barrier);
            }


			DrawScoreBoard(game, player);

			al_flip_display();
		}
}

void HandleKeyUp(int KeyCode, Game &game, Player &player, bool keys[], bool &Exit)
{
    switch (KeyCode)
    {
        case ALLEGRO_KEY_UP:
            keys[UP] = false;
            player.dir = NONE;
            break;
        case ALLEGRO_KEY_DOWN:
            keys[DOWN] = false;
            player.dir = NONE;
            break;
        case ALLEGRO_KEY_LEFT:
            keys[LEFT] = false;
            player.dir = NONE;
            break;
        case ALLEGRO_KEY_RIGHT:
            keys[RIGHT] = false;
            player.dir = NONE;
            break;
        case ALLEGRO_KEY_P:
            game.isPaused = !game.isPaused;
            break;
        case ALLEGRO_KEY_SPACE:
            if(player.isAlive)
            {
                player.isFiring = true;
                Fire(player);
            }
            else
            {
                game.enemies.clear();
                game.isOver = false;
                InitPlayer(player);
            }

            break;
        case ALLEGRO_KEY_ESCAPE:
            Exit = true;
            break;
    }
}

void HandleKeyDown(int KeyCode, bool keys[])
{
    switch (KeyCode)
    {
        case ALLEGRO_KEY_UP:
            keys[UP] = true;
            break;
        case ALLEGRO_KEY_DOWN:
            keys[DOWN] = true;
            break;
        case ALLEGRO_KEY_LEFT:
            keys[LEFT] = true;
            break;
        case ALLEGRO_KEY_RIGHT:
            keys[RIGHT] = true;
            break;
    }
}

void DetectCollisions(Player &player, Game &game)
{
   for(unsigned int j = 0; j < game.enemies.size(); j++)
   {
        for (unsigned int i = 0; i < player.bullets.size(); i++)
        {
            // Detects collisions between bullets and enemies
            DetectCollision(player, game, j, i);
        }

        // Detects collisions between player and enemies and between enemies and plasma barrier (if active)
        DetectCollision(player, game, j);
   }
}

void DetectCollision(Player &player, Game &game, int enemyIndex, int bulletIndex)
{
    Enemy &enemy = game.enemies[enemyIndex];

    if(bulletIndex != -1) // Collisions between player's bullet and enemy
    {
        if(BoundingBoxCollision(player.bullets[bulletIndex], enemy.transform))
        {
            #ifdef DEBUG
                fprintf(stderr, "Enemy killed - DetectCollision\n");
            #endif // DEBUG
            InitExplosion(game, enemy);
            if(enemy.hasPowerUp)
                UpdatePowerUp(game, enemy.transform.x + enemy.transform.width/2, \
                            enemy.transform.y + enemy.transform.height/2, POWERUP_INIT_VEL, true);
            game.enemies.erase(game.enemies.begin() + enemyIndex);
            player.bullets.erase(player.bullets.begin() + bulletIndex);
            game.points++;
            game.enemiesKilledCount++;
        }

    }
    else
    {
        if(player.isAlive) // Collisions between player and enemy
        {
            if(BoundingBoxCollision(player.transform, enemy.transform))
            {
                #ifdef DEBUG
                    fprintf(stderr, "Player killed - DetectCollision\n");
                #endif // DEBUG
                InitExplosion(game, enemy);
                KillPlayer(game, player);
            }

        }

        if(game.isPowerUpSpawned) // Collisions between player and powerup
        {
            if(BoundingBoxCollision(player.transform, game.powerup))
            {
                #ifdef DEBUG
                    fprintf(stderr, "PowerUp collision - DetectCollision\n");
                #endif // DEBUG
                UpdatePowerUpEffect(game, true);
            }
        }

        if(game.isPowerUpActive) // Collisions between enemy and plasma barrier
        {
            Transform barrier;
            barrier.x = 0;
            barrier.y = SCREEN_HEIGHT - PLASMA_BARRIER_HEIGHT;
            barrier.width = SCREEN_WIDTH;
            barrier.height = PLASMA_BARRIER_HEIGHT;
            if(BoundingBoxCollision(barrier, enemy.transform))
            {
                #ifdef DEBUG
                    fprintf(stderr, "Enemy killed with plasma barrier - DetectCollision\n");
                #endif // DEBUG
                InitExplosion(game, enemy);
                game.enemies.erase(game.enemies.begin() + enemyIndex);
                game.points++;
            }
        }

    }

}



void DrawScoreBoard(Game &game, Player &player)
{
    // create a string stream in order to build the string than convert it to string in order to convert it to a *char
    stringstream ss;
    ss << "Points: " << game.points;
    const string tmp = ss.str();
    const char* cstr = tmp.c_str();
    al_draw_text(Font, al_map_rgba_f(1.0, 1.0, 1.0, 1.0), 5, 10, ALLEGRO_ALIGN_LEFT, cstr);

    if(game.record > 0) // write record in the scoreboard
    {
        if(game.points > game.record)
            game.record = game.points;
        stringstream ss;
        ss << "Record: " << game.record;
        const string tmp = ss.str();
        const char* cstr = tmp.c_str();
        al_draw_text(Font, al_map_rgba_f(1.0, 1.0, 1.0, 1.0), 5, 30, ALLEGRO_ALIGN_LEFT, cstr);
    }


    if(game.isOver)
    {
        al_draw_bitmap(GameOverImage, SCREEN_WIDTH/2 - GAME_OVER_SIZE_W/2, SCREEN_HEIGHT/2 - GAME_OVER_SIZE_H/2, 0);
    }
    else
    {
        for(int i = 1; i <= player.lifes; i++)
            al_draw_bitmap(LifesImage, SCREEN_WIDTH - LIFES_IMAGE_SPACE*i, 10, 0);
    }

    if(game.isPaused)
    {
        al_draw_bitmap(GamePausedImage, SCREEN_WIDTH/2 - GAME_OVER_SIZE_W/2, SCREEN_HEIGHT/2 - GAME_OVER_SIZE_H/2, 0);
    }
}

void UpdateBackgrounds(Game &game)
{
    for(int i = 0; i < 2*BG_NUM; i++)
    {
        Transform &bg = game.backgrounds[i];
        MoveTransform(bg, bg.vel, DOWN);
        CheckBgLimit(bg);
    }
}

void DrawBackgrounds(Game &game)
{
    for(int i = 0; i < 2*BG_NUM; i++)
    {
        Transform bg = game.backgrounds[i];
        al_draw_bitmap(bg.image, bg.x, bg.y, 0);
    }

}

void CheckBgLimit(Transform &bg)
{
    if(bg.y >= SCREEN_HEIGHT)
        bg.y = -BG_IMAGE_HEIGHT; // when we are at the bottom we place the background to the top (left corner out of the screen)
}

void UpdatePowerUp(Game &game, int x, int y, int velocity, bool visible)
{
    game.powerup.x = x;
    game.powerup.y = y;
    game.powerup.vel = velocity;

    game.isPowerUpSpawned = visible;

    if(visible)
    {
        #ifdef DEBUG
            fprintf(stderr, "POWERUP SPAWNED! - UpdatePowerUp\n");
        #endif // DEBUG
        al_start_timer(PowerUpTimer);
    }
    else
        al_stop_timer(PowerUpTimer);

}

void UpdatePowerUpEffect(Game &game, bool activated)
{
    game.isPowerUpActive = activated;

    if(activated)
    {
        #ifdef DEBUG
            fprintf(stderr, "POWERUP PICKED UP! - UpdatePowerUpEffect\n");
        #endif // DEBUG

        // if powerup effect was already acitvated we reset its time
        al_stop_timer(PowerUpEffectTimer);
        al_start_timer(PowerUpEffectTimer);
        // if we picked up the powerup we remove it from the screen
        UpdatePowerUp(game, 0, 0, 0, false);
    }
    else
        al_stop_timer(PowerUpEffectTimer);
}

int RecordExist()
{
    ifstream f(RECORD_FILE_NAME);

    if(!f)
        return 0;

    int previousRecord;


    if(!(f>>previousRecord))
        previousRecord = 0;

    return previousRecord;

}

void WriteRecord(const Game &game)
{
    ofstream f(RECORD_FILE_NAME);

    assert(f);

    f<<game.points;
}


void QuitGame(Player &player, Game &game)
{

    if(RecordExist() < game.points) // made a new record?
        WriteRecord(game);

    assert(GameTimer);
    al_destroy_timer(GameTimer);
    assert(Queue);
    al_destroy_event_queue(Queue);
    assert(Display);
    al_destroy_display(Display);
    assert(Font);
    al_destroy_font(Font);

    player.bullets.clear();
    game.enemies.clear();

    #ifdef DEBUG
        fprintf(stderr, "QUIT GAME - QuitGame function");
    #endif // DEBUG
}
