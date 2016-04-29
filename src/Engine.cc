#include <allegro5/allegro5.h>
#include "Data.h"

void InitTransform(Transform &transform, const int x, const int y, const int vel, const int width, \
                    const int height, const int currentFrame = -1, ALLEGRO_BITMAP *image = NULL)
{
    transform.x = x;
    transform.y = y;
    transform.vel = vel;
    transform.width = width;
    transform.height = height;
    transform.currentFrame = currentFrame;

    if(image != NULL)
        transform.image = image;

}
bool BoundingBoxCollision(const Transform &object1, const Transform &object2)
{
    // object 1 is at the right of object 2 OR object 1 is underneath object 2 OR
    // object 2 is at the right of object 1 or object 2 is underneath object 1
    if ((object1.x > object2.x + object2.width) || (object1.y > object2.y + object2.height) || \
        (object2.x > object1.x + object1.width) || (object2.y > object1.y + object1.height))
    {
        // no collision
        return false;
    }

    // collision
    return true;
}

void DrawSprite(const Transform &transform)
{
    ALLEGRO_BITMAP *correctFrame;
    if(transform.currentFrame != -1)
        correctFrame = al_create_sub_bitmap(transform.image, transform.currentFrame*transform.width, 0, \
                        transform.width, transform.height);
    else
        correctFrame = transform.image;

    al_draw_bitmap(correctFrame, transform.x, transform.y, 0);
}

bool UpdateAnimationFrame(Transform &transform, int maxFrame, bool isLoop)
{
    transform.currentFrame++;

    if(transform.currentFrame >= maxFrame)
    {
        transform.currentFrame = 0;

        if(!isLoop)
            return false;
    }

    return true;

}

void MoveTransform(Transform &transform, int velocity, Directions dir, int w, int h, bool hasCollisions)
{
    switch(dir)
    {
        case UP:
            // if we have collisions keep the top corner of the spaceship a little lower than the upper window corner
            if(!hasCollisions || (hasCollisions && transform.y > w/4))
                transform.y -= velocity;
        break;
        case DOWN:
            if(!hasCollisions || (transform.y < SCREEN_HEIGHT - h))
                transform.y += velocity;
        break;
        case LEFT:
         // if we have collisions keep the left spaceship corner inside the window
         if(!hasCollisions || (transform.x > w/4))
            transform.x -= velocity;
        break;
        case RIGHT:
         if(!hasCollisions || (transform.x < SCREEN_WIDTH - w))
            transform.x += velocity;
        break;
	case NONE:
	break;
    }
}


