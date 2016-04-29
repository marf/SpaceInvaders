#include "Player.h"
#include "Engine.h"
#include "Data.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <cstdio>
#include <vector>

void InitPlayer(Player &player)
{
    InitTransform(player.transform, SCREEN_WIDTH/2 - player.transform.width/2, SCREEN_HEIGHT/1.5, PLAYER_INIT_VEL, \
                    PLAYER_SIZE_W, PLAYER_SIZE_H, 0);
    player.dir = NONE;
    player.nextFire = time(0);

    player.lifes = PLAYER_LIFES;

    player.isAlive = true;

}

void MovePlayer(Player &player, const bool keys[])
{
    if(keys[UP])
    {
        player.dir = UP;
    }
    else if(keys[DOWN])
    {
        player.dir = DOWN;
    }

    else if(keys[LEFT])
    {
        player.dir = LEFT;
    }
    else if(keys[RIGHT])
    {
        player.dir = RIGHT;
    }

    MoveTransform(player.transform, player.transform.vel, player.dir, player.transform.width, \
                    player.transform.height, true);

}

void KillPlayer(Game &game, Player &player)
{
    player.bullets.clear();
    player.isAlive = false;
    game.isOver = true;
}

void Fire(Player &player)
{
    time_t now = time(0);

    if(difftime(now, player.nextFire) > 0)
    {
        #ifdef DEBUG
            fprintf(stderr, "FIRE! - Fire\n");
        #endif // DEBUG
        player.nextFire = now + PLAYER_FIRE_RATE;
        Transform bullet;
        InitBullet(bullet, player);
    }
    else
    {
        #ifdef DEBUG
                fprintf(stderr, "RELOADING! - Fire\n");
        #endif // DEBUG
    }

}

void DrawPlayer(const Player &player)
{
    ALLEGRO_BITMAP *correctFrame = al_create_sub_bitmap(player.transform.image, \
                player.transform.currentFrame*player.transform.width, 0, player.transform.width, player.transform.height);
    if(player.dir == LEFT)
        al_draw_rotated_bitmap(correctFrame, 0, 0, player.transform.x, player.transform.y, 50, 0);
    else if(player.dir == RIGHT)
        al_draw_rotated_bitmap(correctFrame, 0, 0, player.transform.x, player.transform.y, -50, 0);
    else
        al_draw_bitmap(correctFrame, player.transform.x, player.transform.y, 0);
}

void InitBullet(Transform &bullet, Player &player)
{
    InitTransform(bullet, player.transform.x + player.transform.width/2 - BULLET_SIZE_W/2, \
                player.transform.y - BULLET_SIZE_H, BULLET_INIT_VEL, BULLET_SIZE_W, BULLET_SIZE_H, -1, player.bulletImage);
    player.bullets.push_back(bullet);
}

void MoveBullets(Player &player)
{
     for (unsigned int i = 0; i < player.bullets.size(); i++)
     {
        Transform &bullet = player.bullets[i];
        MoveTransform(bullet, bullet.vel, UP);

        if(bullet.y < 0)
            player.bullets.erase(player.bullets.begin() + i);
     }
}

void DrawBullets(const Player &player)
{
    // no need of an animation: currentFrame is -1
    for (unsigned int i = 0; i < player.bullets.size(); i++)
    {
        Transform bullet = player.bullets[i];
        DrawSprite(bullet);
    }
}

