#ifndef ZOMBIE_H
#define ZOMBIE_H
#include <string>
#include "LTexture.h"
#include "HitBox.h"
#include "Entity.h"
#include "CollisionHandler.h"
#include "Inventory.h"
#include <math.h>
#include <random>
#include <vector>
#include <utility>
#include <SDL2/SDL.h>
#include "Window.h"
#include "Movable.h"

#define ZOMBIE_VELOCITY 200
#define ZOMBIE_INIT_HP  100
const int ZOMBIE_HEIGHT = 125;
const int ZOMBIE_WIDTH = 75;

typedef enum {
    IDLE,
    MOVE,
    ATTACK,
    DIE
} ZOMBIE_STATE;

class Zombie : public Movable {
public:
    Zombie(int hp = ZOMBIE_INIT_HP, ZOMBIE_STATE state = IDLE);
    virtual ~Zombie();

	void onCollision();

	void collidingProjectile(int damage);

    void generateMove();            // A* movement

    void setStep(int sp);           // set step

    int getStep();                  // get step

    float getEndY();                // get the y-coordinate for the end of the step

    float getEndX();                // get the x-coordinate for the end of the step

    bool isMoving();                // Returns if the zombie should be moving

    void checkMove();               // Does a check to see if zombie should move

    void setEnd(float X, float Y);  // Sets the end coordinates of a zombie step

    std::string getPath();          // get path

    void setPath(std::string pth);  // set path

    int getMoveDir();               // get move direction

    bool checkBounds(const float& x, const float& y) const;  // boundary checks
    // A* path
    std::string generatePath(const int& xStart, const int& yStart,
                             const int& xDest, const int& yDest);

private:
    int hp;             // health points of zombie
    int step;           // Number of steps zombie has taken in path
    //bool moving;      // Whether or not zombie is moving
    std::string path;   // A* path zombie should follow
    float endX;         // X coordinate of the end of a step
    float endY;         // Y coordinate of the end of a step
    ZOMBIE_STATE state; // 0 - idle, 1 - move, 2 - attack, 3 - die
};

#endif
