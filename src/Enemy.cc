#include <cstdio>
#include "Engine.h"
#include "Data.h"
#include "Player.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

void GenerateFreeCoord(int &x, Game &game)
{
    x = (rand() % (SCREEN_WIDTH - 2*ENEMY_SIZE_W)) + ENEMY_SIZE_W; // leave space from borders
    for (unsigned int i = 0; i<game.enemies.size(); i++)
    {
        // Check if enemy's spawn position is between a delta distance between other enemies
        if(game.enemies[i].transform.x > x-ENEMY_DELTA_SPAWN && game.enemies[i].transform.x < x+ENEMY_DELTA_SPAWN )
        {
           GenerateFreeCoord(x, game); // regenerate coord if enemy - delta < x < enemy + delta
        }
    }


}

void InitEnemy(Game &game, ALLEGRO_BITMAP *enemyImage)
{
    assert(enemyImage);

    Enemy enemy;
    int x;
    GenerateFreeCoord(x, game);
    int velocity = ENEMY_INIT_VEL;

    game.enemiesCount++;

    if(game.enemiesKilledCount > 0 && game.enemiesKilledCount%5 == 0)
            game.increaseVel = true;

    if(game.increaseVel)
        velocity *= 2;

    InitTransform(enemy.transform, x, -50, velocity, ENEMY_SIZE_W, ENEMY_SIZE_H, 0, enemyImage);

    if((game.enemiesCount%POWERUP_FREQUENCY) == 0)
        enemy.hasPowerUp = true;
    else
        enemy.hasPowerUp = false;
    game.enemies.push_back(enemy);

    #ifdef DEBUG
        fprintf(stderr, "ENEMY SPAWNED X = %d - InitEnemy\n", enemy.transform.x);
    #endif // DEBUG
}

void UpdateEnemiesFrame(Game &game)
{
    for (unsigned int i = 0; i<game.enemies.size(); i++)
        UpdateAnimationFrame(game.enemies[i].transform, ENEMY_FRAMES, true);
}

void DrawEnemies(const Game &game)
{
   for (unsigned int i = 0; i<game.enemies.size(); i++)
   {
        Enemy enemy = game.enemies[i];
        DrawSprite(enemy.transform);
   }
}

void MoveEnemies(Game &game, Player &player)
{
     for (unsigned int i = 0; i < game.enemies.size(); i++)
     {
        Enemy &enemy = game.enemies[i];

        MoveTransform(enemy.transform, enemy.transform.vel, DOWN);

        if(enemy.transform.y >= SCREEN_HEIGHT) // enemy crossed the area
        {
            game.enemies.erase(game.enemies.begin() + i);

            if(--player.lifes <= 0)
            {
                player.isAlive = false;
                game.isOver = true;
            }
        }
     }
}

void InitExplosion(Game &game, const Enemy &enemy)
{
    #ifdef DEBUG
        fprintf(stderr, "ADDED EXPLOSION - InitExplosion\n");
    #endif // DEBUG

    Transform explosion;
    InitTransform(explosion, enemy.transform.x, enemy.transform.y + enemy.transform.height/2, 0, \
                    EXPLOSION_SIZE_W, EXPLOSION_SIZE_H, 0, game.explosionImage);
    game.explosions.push_back(explosion);
}

void UpdateExplosions(Game &game)
{
    for (unsigned int i = 0; i < game.explosions.size(); i++)
    {
        if(!UpdateAnimationFrame(game.explosions[i], EXPLOSION_FRAMES, false))
        {
            game.explosions.erase(game.explosions.begin() + i);

            #ifdef DEBUG
                fprintf(stderr, "END OF EXPLOSION - UpdateExplosions\n");
            #endif // DEBUG
        }
    }
}

void DrawExplosions(const Game &game)
{
    for (unsigned int i = 0; i < game.explosions.size(); i++)
    {
        Transform explosion = game.explosions[i];
        DrawSprite(explosion);
    }
}

