#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <string>
#include <math.h>
#include <random>
#include <vector>
#include <utility>
#include <SDL2/SDL.h>
#include "../collision/HitBox.h"
#include "../basic/Entity.h"
#include "../collision/CollisionHandler.h"
#include "../inventory/Inventory.h"
#include "../collision/Quadtree.h"
#include "../buildings/Base.h"
#include "../view/Window.h"
#include "../basic/Movable.h"

typedef std::pair<float, float> Point;

static constexpr int ZOMBIE_HEIGHT   = 125;
static constexpr int ZOMBIE_WIDTH    = 75;
static constexpr int ZOMBIE_INIT_HP  = 100;
static constexpr int ZOMBIE_VELOCITY = 150;
static constexpr int ZOMBIE_FRAMES   = 30;

// block threshold - check if zombie is blocked
static constexpr float BLOCK_THRESHOLD = 0.5;

/* 8 possible directions combining left, right, up, down.
 * Fred Yang
 * Feb 14
 */
enum class ZombieDirection : int {
    DIR_R,
    DIR_RD,
    DIR_D,
    DIR_LD,
    DIR_L,
    DIR_LU,
    DIR_U,
    DIR_RU,
    DIR_INVALID = -1
};

/* Cardinal directions for setting angles, one angle for each movement direction.
 * Robert Arendac
 * March 14
 */
enum class ZombieAngles : int {
    NORTH = 0,
    NORTHEAST = 45,
    EAST = 90,
    SOUTHEAST = 135,
    SOUTH = 180,
    SOUTHWEST = 225,
    WEST = 270,
    NORTHWEST = 315
};

/* zombie states, change when you want zombie to take a different action.
 * Eg. go from moving to attacking
 * Fred Yang
 * March 14
 */
enum class ZombieState {
    ZOMBIE_IDLE,
    ZOMBIE_MOVE,
    ZOMBIE_ATTACK,
    ZOMBIE_DIE
};

class Zombie : public Movable {
public:
    Zombie(int32_t id, const SDL_Rect &dest, const SDL_Rect &movementSize, const SDL_Rect &projectileSize,
        const SDL_Rect &damageSize, int health = ZOMBIE_INIT_HP, ZombieState state = ZombieState::ZOMBIE_IDLE,
        int step = 0, ZombieDirection dir = ZombieDirection::DIR_INVALID, int frame = ZOMBIE_FRAMES, float range = 400.0f);

    virtual ~Zombie();

    void onCollision();

    void collidingProjectile(int damage);
    
    void move(float moveX, float moveY, CollisionHandler& ch) override;  // move method

    void generateMove();                    // A* movement

    bool isMoving() const;                  // Returns if the zombie should be moving

    int detectObj() const;                  // detect objects in vicinity
    
    void attack();                          // attack/destroy marines, turrets, or barricades
    
    void die();                             // zombie die method

    ZombieDirection getMoveDir();           // get move direction

    // A* path
    std::string generatePath(const Point& start);
    std::string generatePath(const Point& start, const Point& dest);

    /**
     * Set steps taken
     * Fred Yang
     * Feb 14
     */
    void setStep(const int sp) {
        step = sp;
    }

    /**
     * Get steps taken
     * Fred Yang
     * Feb 14
     */
    int getStep() const {
        return step;
    }

    /**
     * Set state
     * Fred Yang
     * March 14
     */
    void setState(const ZombieState newState) {
        state = newState;
    }

    /**
     * Get state
     * Fred Yang
     * March 14
     */
    ZombieState getState() const {
        return state;
    }

    /**
     * Get A* path
     * Fred Yang
     * Feb 14
     */
    string getPath() const {
        return path;
    }

    /**
     * Set A* path
     * Fred Yang
     * Feb 14
     */
    void setPath(const string pth) {
        path = pth;
    }

    /**
     * Set direction
     * Fred Yang
     * March 14
     */
    void setCurDir(const ZombieDirection d) {
        dir = d;
    }

    /**
     * Get current direction
     * Fred Yang
     * March 14
     */
    ZombieDirection getCurDir() const {
        return dir;
    }

    /**
     * Set frame
     * Fred Yang
     * March 14
     */
    void setCurFrame(const int frm) {
        frame = frm;
    }

    /**
     * Get current frame
     * Fred Yang
     * March 14
     */
    int getCurFrame() const {
        return frame;
    }

    // returns the zombie's range.
    // Jamie, 2017-03-27.
    float getRange() const;

    // find a target if not found, return false.
    // Jamie, 2017-04-04.
    bool findTarget();

private:
    int health;         // health points of zombie
    std::string path;   // A* path zombie should follow
    ZombieState state;  // 0 - idle, 1 - move, 2 - attack, 3 - die
    int step;           // Number of steps zombie has taken in path
    ZombieDirection dir;// moving direction
    int frame;          // frames per tile
    float range;        // zombie's range.
};

#endif
