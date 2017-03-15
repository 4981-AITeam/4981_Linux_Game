#ifndef ZOMBIE_H
#define ZOMBIE_H
#include <string>
#include "../sprites/LTexture.h"
#include "../collision/HitBox.h"
#include "../basic/Entity.h"
#include "../collision/CollisionHandler.h"
#include "../inventory/Inventory.h"
#include "../buildings/Base.h"
#include <math.h>
#include <random>
#include <vector>
#include <utility>
#include "../view/Window.h"
#include "../basic/Movable.h"

typedef std::pair<float, float> Point;

static constexpr int ZOMBIE_HEIGHT   = 125;
static constexpr int ZOMBIE_WIDTH    = 75;
static constexpr int ZOMBIE_INIT_HP  = 100;
static constexpr int ZOMBIE_VELOCITY = 100;
static constexpr int ZOMBIE_FRAMES   = 100;

// 8 possible directions
static constexpr int DIR_R   = 0;
static constexpr int DIR_RD  = 1;
static constexpr int DIR_D   = 2;
static constexpr int DIR_LD  = 3;
static constexpr int DIR_L   = 4;
static constexpr int DIR_LU  = 5;
static constexpr int DIR_U   = 6;
static constexpr int DIR_RU  = 7;

// Cardinal directions for setting angles
static constexpr int NORTH     = 0;
static constexpr int NORTHEAST = 45;
static constexpr int EAST      = 90;
static constexpr int SOUTHEAST = 135;
static constexpr int SOUTH     = 180;
static constexpr int SOUTHWEST = 225;
static constexpr int WEST      = 270;
static constexpr int NORTHWEST = 315;

// overlapped value
static constexpr float OVERLAP = 0.1f;

// zombie state
typedef enum {
    ZOMBIE_IDLE,
    ZOMBIE_MOVE,
    ZOMBIE_ATTACK,
    ZOMBIE_DIE
} ZombieState;

class Zombie : public Movable {
public:
    Zombie(int health = ZOMBIE_INIT_HP, ZombieState state = ZOMBIE_IDLE, int step = 0,
           int dir = -1, int frame = ZOMBIE_FRAMES);
    virtual ~Zombie();

    void onCollision();

    void collidingProjectile(int damage);

    void generateMove();            // A* movement

    void setStep(const int step_);  // set step

    int getStep() const;            // get step

    void setState(const ZombieState state_); // set state

    ZombieState getState() const;  // get state

    bool isMoving();                // Returns if the zombie should be moving

    bool checkBase();               // checks if the zombie already arrived at the base

    std::string getPath() const;    // get path

    void setPath(const std::string path_); // set path

    int getMoveDir();               // get move direction

    void setCurDir(const int dir_); // set current direction

    int getCurDir() const;          // get current direction

    void setCurFrame(const int frame_); // set current frame

    int getCurFrame() const;        // get current frame

    bool checkBounds(const Point& point) const;  // boundary checks

    // A* path
    std::string generatePath(const Point& start);
    std::string generatePath(const Point& start, const Point& dest);

    // overlapped method for 2 rects
    bool overlapped(const SDL_Rect& rect1, const SDL_Rect& rect2, const float overlap);

private:
    int health;         // health points of zombie
    std::string path;   // A* path zombie should follow
    ZombieState state; // 0 - idle, 1 - move, 2 - attack, 3 - die
    int step;           // Number of steps zombie has taken in path
    int dir;            // moving direction
    int frame;          // frames per tile
};

#endif
