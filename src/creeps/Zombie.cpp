#include "Node.h"
#include "Zombie.h"
#include "../log/log.h"
#include "../game/GameManager.h"
#include <utility>
using namespace std;

Zombie::Zombie(int health, ZombieState state, int step, ZombieDirection dir, int frame)
      : Movable(ZOMBIE_VELOCITY), health(health), state(state), step(step),
        dir(dir), frame(frame) {
    logv("Create Zombie\n");
}

Zombie::~Zombie() {
    logv("Destroy Zombie\n");
}

/**
 * Get move direction
 * Fred Yang, Robert Arendac
 * February 14
 */
ZombieDirection Zombie::getMoveDir() {
    if (frame > 0) {
        return dir;
    }

    string pth = generatePath(Point(getX(),getY()));

    return static_cast<ZombieDirection>(pth.length() > 0 ? stoi(pth.substr(0,1)) : -1);
}

void Zombie::onCollision() {
    // Do nothing for now
}

void Zombie::collidingProjectile(int damage) {
    health -= damage;
}

/*
 * Returns if the zombie is moving
 * Robert Arendac
 * March 7
*/
bool Zombie::isMoving() const {
    return (state == ZombieState::ZOMBIE_MOVE);
}

/*
 * Does a check to see if the zombie already arrived at the target.
 * In theory, zombies will only have a movement collision with a target
 * as their pathfinding should walk around obstacles.
 * Robert Arendac
 * March 7
*/
bool Zombie::checkTarget() {
    SDL_Rect rect, rectBase;
    rect.w = ZOMBIE_WIDTH;
    rect.h = ZOMBIE_HEIGHT;
    rect.x = getX();
    rect.y = getY();
    rectBase.w = BASE_WIDTH + TILE_SIZE;
    rectBase.h = BASE_HEIGHT + TILE_SIZE/2;
    rectBase.x = BASE_X - TILE_SIZE/2;
    rectBase.y = BASE_Y - TILE_SIZE/4;

    return overlapped(rect, rectBase, OVERLAP);
    
    // need to check with GM, why the handler below doesn't work
    //return GameManager::instance()->getCollisionHandler().detectMovementCollision(this);
}

/*
 * overriden move method, preventing zombies from blocking
 * Fred Yang,  Robert Arendac
 * March 15
*/
void Zombie::move(float moveX, float moveY, CollisionHandler& ch){
    ZombieDirection newDir = ZombieDirection::DIR_INVALID;
    ZombieDirection nextDir = ZombieDirection::DIR_INVALID;

    float oldX = getX();
    float oldY = getY();

    //Move the Movable left or right
    setX(getX() + moveX);

    if (ch.detectMovementCollision(this)) {
        setX(getX() - moveX);
    }

    //Move the Movable up or down
    setY(getY() + moveY);

    if (ch.detectMovementCollision(this)) {
        setY(getY() - moveY);
    }

    float curX = getX();
    float curY = getY();
    float dist = sqrt((curX - oldX)*(curX - oldX) + (curY - oldY)*(curY - oldY));

    // zombie blocked
    if (dist < BLOCK_THRESHOLD) {
        string pth = getPath();
        size_t found = pth.find_first_not_of('0' + static_cast<int>(dir));
        
        if (found != string::npos) {
          nextDir = static_cast<ZombieDirection>(stoi(pth.substr(found,1)));
        }

        // If blocked, searching for better direction
        switch (dir) {
            case ZombieDirection::DIR_R:
                newDir = (nextDir == ZombieDirection::DIR_RD ? 
                          ZombieDirection::DIR_D : ZombieDirection::DIR_U);
                break;
            case ZombieDirection::DIR_RD:
                newDir = (nextDir == ZombieDirection::DIR_D ? 
                          ZombieDirection::DIR_LD : ZombieDirection::DIR_RU);
                break;
            case ZombieDirection::DIR_D:
                newDir = (nextDir == ZombieDirection::DIR_LD ?
                          ZombieDirection::DIR_L : ZombieDirection::DIR_R);
                break;
            case ZombieDirection::DIR_LD:
                newDir = (nextDir == ZombieDirection::DIR_L ?
                          ZombieDirection::DIR_LU : ZombieDirection::DIR_RD);
                break;
            case ZombieDirection::DIR_L:
                newDir = (nextDir == ZombieDirection::DIR_LU ? 
                          ZombieDirection::DIR_U : ZombieDirection::DIR_D);
                break;
            case ZombieDirection::DIR_LU:
                newDir = (nextDir == ZombieDirection::DIR_U ?
                          ZombieDirection::DIR_RU : ZombieDirection::DIR_LD);
                break;
            case ZombieDirection::DIR_U:
                newDir = (nextDir == ZombieDirection::DIR_RU ?
                          ZombieDirection::DIR_R : ZombieDirection::DIR_L);
                break;
            case ZombieDirection::DIR_RU:
                newDir = (nextDir == ZombieDirection::DIR_R ?
                          ZombieDirection::DIR_RD : ZombieDirection::DIR_LU);
                break;
            case ZombieDirection::DIR_INVALID:
                break;
        }

        // set current direction
        setCurDir(newDir);
    }
}

/**
 * Get the direction of the zombie and take a step in the appropriate direction
 * Rob, Fred
 * March 13
*/
void Zombie::generateMove() {
    //Direction zombie is moving
    ZombieDirection d = getMoveDir();
    
    float startX = getX();
    float startY = getY();

    // Path is empty, shouldn't move
    if (d == ZombieDirection::DIR_INVALID || checkTarget()) {
        if (frame > 0) {
            --frame;
        }

        // Changed to attack state once attack code is ready
        if (state != ZombieState::ZOMBIE_IDLE) {
            setState(ZombieState::ZOMBIE_IDLE);
        }

        return;
    }

    // Each case will check if the zombie is within bounds before moving
    switch(d) {
        case ZombieDirection::DIR_R:
            if (checkBounds(Point(startX + ZOMBIE_VELOCITY, startY))) {
                setDX(ZOMBIE_VELOCITY);
                setDY(0);
                setAngle(static_cast<double>(ZombieAngles::EAST));
            }
            break;
        case ZombieDirection::DIR_RD:
            if (checkBounds(Point(startX + ZOMBIE_VELOCITY, startY + ZOMBIE_VELOCITY))) {
                setDX(ZOMBIE_VELOCITY);
                setDY(ZOMBIE_VELOCITY);
                setAngle(static_cast<double>(ZombieAngles::SOUTHEAST));
            }
            break;
        case ZombieDirection::DIR_D:
            if (checkBounds(Point(startX, startY + ZOMBIE_VELOCITY))) {
                setDX(0);
                setDY(ZOMBIE_VELOCITY);
                setAngle(static_cast<double>(ZombieAngles::SOUTH));
            }
            break;
        case ZombieDirection::DIR_LD:
            if (checkBounds(Point(startX - ZOMBIE_VELOCITY, startY + ZOMBIE_VELOCITY))) {
                setDX(-ZOMBIE_VELOCITY);
                setDY(ZOMBIE_VELOCITY);
                setAngle(static_cast<double>(ZombieAngles::SOUTHWEST));
            }
            break;
        case ZombieDirection::DIR_L:
            if (checkBounds(Point(startX - ZOMBIE_VELOCITY, startY))) {
                setDX(-ZOMBIE_VELOCITY);
                setDY(0);
                setAngle(static_cast<double>(ZombieAngles::WEST));
            }
            break;
        case ZombieDirection::DIR_LU:
            if (checkBounds(Point(startX - ZOMBIE_VELOCITY, startY - ZOMBIE_VELOCITY))) {
                setDX(-ZOMBIE_VELOCITY);
                setDY(-ZOMBIE_VELOCITY);
                setAngle(static_cast<double>(ZombieAngles::NORTHWEST));
            }
            break;
        case ZombieDirection::DIR_U:
            if (checkBounds(Point(startX, startY - ZOMBIE_VELOCITY))) {
                setDX(0);
                setDY(-ZOMBIE_VELOCITY);
                setAngle(static_cast<double>(ZombieAngles::NORTH));
            }
            break;
        case ZombieDirection::DIR_RU:
            if (checkBounds(Point(startX + ZOMBIE_VELOCITY, startY - ZOMBIE_VELOCITY))) {
                setDX(ZOMBIE_VELOCITY);
                setDY(-ZOMBIE_VELOCITY);
                setAngle(static_cast<double>(ZombieAngles::NORTHEAST));
            }
            break;
        case ZombieDirection::DIR_INVALID:  // Shouldn't ever happens, gets rid of warning
            break;
    }

    if (frame > 0) {
        --frame;
    } else {
        setCurFrame(ZOMBIE_FRAMES);
        ++step;
    }

    setCurDir(d);
    setState(ZombieState::ZOMBIE_MOVE);
}

/**
 * A* algo generates a string of direction digits.
 * Fred Yang
 * March 15
 */
string Zombie::generatePath(const Point& start) {
    return generatePath(start, Point(MAP_WIDTH/2, MAP_HEIGHT/2));
}

/**
 * A* algo generates a string of direction digits.
 * Fred Yang
 * Feb 14
 */
string Zombie::generatePath(const Point& start, const Point& dest) {
    // temp index
    int i, j;
    char c;

    // priority queue index
    int index = 0;

    // row & column index
    int curRow, curCol;
    int newRow, newCol;

    // path to be generated
    string path;

    // current node & child node
    static Node curNode;
    static Node childNode;

    // priority queue
    static priority_queue<Node> pq[2];

    // reset the node maps
    memset(closedNodes, 0, sizeof(int) * ROWS * COLS);
    memset(openNodes, 0, sizeof(int) * ROWS * COLS);
    memset(dirMap, 0, sizeof(int) * ROWS * COLS);

    int xNodeStart = static_cast<int> (start.second + TILE_OFFSET) / TILE_SIZE;
    int yNodeStart = static_cast<int> (start.first + TILE_OFFSET) / TILE_SIZE;
    int xNodeDest = static_cast<int> (dest.second + TILE_OFFSET) / TILE_SIZE - 1;
    int yNodeDest = static_cast<int> (dest.first + TILE_OFFSET) / TILE_SIZE - 1;

    // create the start node and push into open list
    curNode = Node(xNodeStart, yNodeStart, 0, 0);
    curNode.updatePriority(xNodeDest, yNodeDest);
    pq[index].push(curNode);

    // A* path finding
    while (!pq[index].empty()) {
        // get the current node with the highest priority from open list
        curNode = Node(pq[index].top().getXPos(), pq[index].top().getYPos(),
                       pq[index].top().getLevel(), pq[index].top().getPriority());

        curRow = curNode.getXPos();
        curCol = curNode.getYPos();

        // remove the node from the open list
        pq[index].pop();

        // mark it on open/close map
        openNodes[curRow][curCol] = 0;
        closedNodes[curRow][curCol] = 1;

        // quit searching when the destination is reached
        if (curRow == xNodeDest && curCol == yNodeDest) {
            // generate the path from destination to start
            // by following the directions
            path = "";
            while (!(curRow == xNodeStart && curCol == yNodeStart)) {
                j = dirMap[curRow][curCol];
                c = '0' + (j + DIR_CAP/2)%DIR_CAP;
                path = c + path;
                curRow += MY[j];
                curCol += MX[j];
            }

            // empty the leftover nodes
            while (!pq[index].empty()) {
                 pq[index].pop();
            }

            setPath(path);
            return path;
        }

        // traverse neighbors
        for (i = 0; i < DIR_CAP;i++) {
            // neighbor coordinates
            newRow = curRow + MY[i];
            newCol = curCol + MX[i];

            // not evaluated & not outside (bound checking)
            if (!(newRow < 0 || newRow > COLS - 1 || newCol < 0 || newCol > ROWS - 1
                || gameMap[newRow][newCol] >= 1 || closedNodes[newRow][newCol] == 1)) {

                // generate a child node
                childNode = Node(newRow, newCol, curNode.getLevel(), curNode.getPriority());
                childNode.nextLevel(i);
                childNode.updatePriority(xNodeDest, yNodeDest);

                // if it is not in the open list then add into that
                if (openNodes[newRow][newCol] == 0) {
                    openNodes[newRow][newCol] = childNode.getPriority();
                    pq[index].push(childNode);
                    
                    // update the parent direction info
                    dirMap[newRow][newCol] = (i + DIR_CAP/2)%DIR_CAP;
                } else if (openNodes[newRow][newCol] > childNode.getPriority()) {
                    // update the priority info
                    openNodes[newRow][newCol] = childNode.getPriority();
                    
                    // update the parent direction info
                    dirMap[newRow][newCol] = (i + DIR_CAP/2)%DIR_CAP;

                    // use a backup queue to put the best node (with highest priority)
                    // onto the top of the queue, which can be chosen later to build the path.
                    while (!(pq[index].top().getXPos() == newRow &&
                           pq[index].top().getYPos() == newCol)) {
                        pq[1-index].push(pq[index].top());
                        pq[index].pop();
                    }

                    pq[index].pop();

                    if (pq[index].size() > pq[1-index].size()) {
                        index = 1 - index;
                    }

                    while (!pq[index].empty()) {
                        pq[1-index].push(pq[index].top());
                        pq[index].pop();
                    }

                    index = 1 - index;
                    pq[index].push(childNode);
                }
            }
        }
    }

    return ""; // no route found
}

/**
 * Check to see if the zombie is still within the screen bounds before moving
 * Fred Yang
 * Feb 14
 */
bool Zombie::checkBounds(const Point& point) const {
    return (!(point.first < TILE_SIZE - TILE_OFFSET
            || point.second < TILE_SIZE - TILE_OFFSET
            || point.first > MAP_WIDTH + TILE_OFFSET - TILE_SIZE
            || point.second > MAP_HEIGHT + TILE_OFFSET - TILE_SIZE));
}

/**
 * Check to see if two rectangles overlapped
 * Fred Yang
 * Feb 14
 */
bool Zombie::overlapped(const SDL_Rect& rect1, const SDL_Rect& rect2,
                        const float overlap) {
    int bigX    = 0,
        bigY    = 0,
        smallX  = 0,
        smallY  = 0,
        width   = 0,
        height  = 0;

    // which one is bigger?
    if (rect1.x >= rect2.x) {
        bigX = rect1.x;
        smallX = rect2.x;
        width = rect2.w;
    } else {
        bigX = rect2.x;
        smallX = rect1.x;
        width = rect1.w;
    }

    if (rect1.y >= rect2.y) {
        bigY = rect1.y;
        smallY = rect2.y;
        height = rect2.h;
    } else {
        bigY = rect2.y;
        smallY = rect1.y;
        height = rect1.h;
    }

    // now checks overlapped
    if (bigX >= smallX && bigX <= smallX + width - 1 &&
        bigY >= smallY && bigY <= smallY + height - 1) {
        float fWidth = width - bigX + smallX;
        float fHeight = height - bigY + smallY;
        if (fWidth * fHeight / (rect2.w * rect2.h) >= OVERLAP) {
            return true;
        }
    }

    return false;
}


