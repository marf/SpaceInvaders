#ifndef Engine
#define Engine

#include <allegro5/allegro5.h>
#include "Data.h"

/** Inits a transform variable
* @param[in] transform is the transform we want to initialize
* @param[in] x is the initial x coord of the transform
* @param[in] y is the initial y coord of the transform
* @param[in] vel is the vel of the transform
* @param[in] width is the width of the transform
* @param[in] height is the height of the transform
* @param[in] currentFrame is the initial current frame of the transform (default value: -1, set it to 0 if the transform has animations)
* @param[in] image is the bitmap image of the transform (if it was already set leave this blank)
* Takes an optional param currentFrame that has to be set to 0 if the transform has animations
*/
void InitTransform(Transform &transform, const int x, const int y, const int vel, const int width, \
                    int height, const int currentFrame = -1, ALLEGRO_BITMAP *image = NULL);

/**
* Detects a collision between two objects using the BoundingBox method
* @param[in] object1 is the transform with stores the position and size of the first object
* @param[in] object2 is the transform with stores the position and size of the second object
*
* @return Returns if the two objects are colliding
*
* Checks if the first object is at the right of the second object or if the first object is underneath the second object
* or if the second object is at the right of the first object or if the second object is underneath the first object.
* In this case we have no collision.
*/
bool BoundingBoxCollision(const Transform &object1, const Transform &object2);

/**
* Draws the correct sprite of an animation or draws the object if transform.currentFrame (it's optional) is -1 (object hasn't animations)
* @param[in] transform is used to access to the image, size and position of the object
*
*/
void DrawSprite(const Transform &transform);

/** Updates the animation of an object
* @param[in] transform is used to update its current frame
* @param[in] maxFrame is used to check when we have reached the last frame
* @param[in] isLoop is used to create a looping animation or to end the animation
* @return Returns false if we have finished the animation and isLoop is false
*
* If the frame is the last one  and isLoop is true we set the current frame to 0 in order to create a loop.
* If isLoop is false and we have reached the last frame of the animation the function returns false.
*/
bool UpdateAnimationFrame(Transform &transform, int maxFrame, bool isLoop);

/** Moves a transform based on its velocity (optional this function could avoid the transform from going outside the window)
* @param[in] transform is the object we want to move
* @param[in] velocity is the velocity of the object
* @param[in] dir is the direction of the object
* @param[in] optional width of the object (used for collisions)
* @param[in] optional height of the object (used for collisions)
* @param[in] optional bool that if it is set to true avoids the object from going outside the window
*
* If hasCollisions is true the object isn't moved if it's near the borders of the window
*/
void MoveTransform(Transform &transform, int velocity, Directions dir, int w = 0, int h = 0, bool hasCollisions = false);

#endif // Engine
