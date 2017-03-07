#include "Node.h"
#include "Zombie.h"
#include <math.h>
#include <random>
#include <utility>
using namespace std;

#define PI 3.14159265
#define ZOMBIE_VELOCITY 200

Zombie::Zombie(int health, int state) : Movable(ZOMBIE_VELOCITY), health(health), state(state) {
    setAngle(0); //Totally arbitrary right now
    printf("Create Zombie\n");
}

Zombie::~Zombie() {
    printf("Destory Zombie\n");
}

// Get step
int Zombie::getStep() {
    return step;
}

// Set step
void Zombie::setStep(int sp) {
    step = sp;
}

/*
Get move direction
Fred
February
*/
int Zombie::getDir() {
    int sp = this->getStep();
    string pth = this->getPath();
    //char c = pth.at(sp);
    return (sp < (int) pth.length() ? stoi(pth.substr(sp,1)) : -1);
}

// Set path
void Zombie::setPath(string pth) {
    path = pth;
}

// Get path
string Zombie::getPath() {
    return path;
}

void Zombie::onCollision() {
    // Do nothing for now
}

void Zombie::collidingProjectile(int damage) {
    health = health - damage;
}

/*
Get the direction of the zombie and take a step
Rob, Fred
Feb - Ongoing
*/
void Zombie::generateMove() {
    int d = getDir();
    if (d < 0) return;

    float x = getDX();
    float y = getDY();

    switch(d) {
        case DIR_R:
            if (checkBound(x + STEP_SPAN, y)) {
                setDX(x + STEP_SPAN);
                step++;
                setAngle(EAST);
            }
            break;
        case DIR_RD:
            if (checkBound(x + STEP_SPAN, y + STEP_SPAN)) {
                setDX(x + STEP_SPAN);
                setDY(y + STEP_SPAN);
                step++;
                setAngle(SOUTHEAST);
            }
            break;
        case DIR_D:
            if (checkBound(x, y + STEP_SPAN)) {
                setDY(y + STEP_SPAN);
                step++;
                setAngle(SOUTH);
            }
            break;
        case DIR_LD:
            if (checkBound(x - STEP_SPAN, y + STEP_SPAN)) {
                setDX(x - STEP_SPAN);
                setDY(y + STEP_SPAN);
                step++;
                setAngle(SOUTHWEST);
            }
            break;
        case DIR_L:
            if (checkBound(x - STEP_SPAN, y)) {
                setDX(x - STEP_SPAN);
                step++;
                setAngle(WEST);
            }
            break;
        case DIR_LU:
            if (checkBound(x - STEP_SPAN, y - STEP_SPAN)) {
                setDX(x - STEP_SPAN);
                setDY(y - STEP_SPAN);
                step++;
                setAngle(NORTHWEST);
            }
            break;
        case DIR_U:
            if (checkBound(x, y - STEP_SPAN)) {
                setDY(y - STEP_SPAN);
                step++;
                setAngle(NORTH);
            }
            break;
        case DIR_RU:
            if (checkBound(x + STEP_SPAN, y - STEP_SPAN)) {
                setDX(x + STEP_SPAN);
                setDY(y - STEP_SPAN);
                step++;
                setAngle(NORTHEAST);
            }
            break;
        default:
            break;
    }
}

// A* algo generates a string of direction digits.
string Zombie::generatePath(const int& xStart, const int& yStart,
                            const int& xDest, const int& yDest) {
    // priority queue index
    int index = 0;

    // temp index
    int i, j;

    // row / column index
    int x, y, xdx, ydy;

    // temp char
    char c;

    // path
    string path;

    // current node & child node
    static Node* curNode;
    static Node* childNode;

    // priority queue
    static priority_queue<Node> pq[2];

    // reset the node maps
    for(i = 0; i < row; i++)
    {
        for(j = 0; j < col; j++)
        {
            closedNodes[i][j] = 0;
            openNodes[i][j] = 0;
        }
    }

    // create the start node and push into open list
    curNode = new Node(xStart, yStart, 0, 0);
    curNode->updatePriority(xDest, yDest);
    pq[index].push(*curNode);

    // A* path finding
    while(!pq[index].empty())
    {
        // get the current node with the highest priority
        // from open list
        curNode = new Node(pq[index].top().getXPos(), pq[index].top().getYPos(),
                           pq[index].top().getLevel(), pq[index].top().getPriority());

        x = curNode->getXPos();
        y = curNode->getYPos();

        // remove the node from the open list
        pq[index].pop();
        openNodes[x][y] = 0;

        // mark it on the closed nodes map
        closedNodes[x][y] = 1;

        // quit searching when the destination is reached
        if(x == xDest && y == yDest)
        {
            // generate the path from destination to start
            // by following the directions
            path = "";
            while(!(x == xStart && y == yStart))
            {
                j = dirMap[x][y];
                c = '0' + (j + DIR_CAP/2)%DIR_CAP;
                path = c + path;
                x += mx[j];
                y += my[j];
            }
            // garbage collection
            delete curNode;

            // empty the leftover nodes
            while(!pq[index].empty()) pq[index].pop();
            setPath(path);
            return path;
        }

        // traverse neighbors
        for(i = 0; i < DIR_CAP;i++)
        {
            // neighbor coordinates
            xdx = x + mx[i];
            ydy = y + my[i];

            // not evaluated & not outside (bound checking)
            if(!(xdx < 0 || xdx > col -1 || ydy < 0 || ydy > row - 1
                || map[xdx][ydy] == 1 || closedNodes[xdx][ydy] == 1))
            {
                // generate a child node
                childNode = new Node(xdx, ydy, curNode->getLevel(), curNode->getPriority());
                childNode->nextLevel(i);
                childNode->updatePriority(xDest, yDest);

                // if it is not in the open list then add into that
                if(openNodes[xdx][ydy] == 0)
                {
                    openNodes[xdx][ydy] = childNode->getPriority();
                    pq[index].push(*childNode);
                    // update the parent direction info
                    dirMap[xdx][ydy] = (i + DIR_CAP/2)%DIR_CAP;
                }
                else if(openNodes[xdx][ydy] > childNode->getPriority())
                {
                    // update the priority info
                    openNodes[xdx][ydy]= childNode->getPriority();
                    // update the parent direction info
                    dirMap[xdx][ydy] = (i + DIR_CAP/2)%DIR_CAP;

                    // use a queue and a backup queue to put the best node (with highest priority)
                    // on the top of the queue, which can be chosen later on to build the path.
                    while(!(pq[index].top().getXPos() == xdx &&
                           pq[index].top().getYPos() == ydy))
                    {
                        pq[1-index].push(pq[index].top());
                        pq[index].pop();
                    }

                    pq[index].pop();

                    // switch to pq with smaller size
                    if(pq[index].size() > pq[1-index].size())
                    {
                        index = 1 - index;
                    }

                    while(!pq[index].empty())
                    {
                        pq[1-index].push(pq[index].top());
                        pq[index].pop();
                    }
                    index = 1 - index;
                    pq[index].push(*childNode);
                }
                else delete childNode;
            }
        }
        delete curNode;
    }

    return ""; // no route found
}

bool Zombie::checkBound(float x, float y) {
    if (x < 0 || x > SCREEN_W || y < 0 || y > SCREEN_H)
        return false;
    return true;
}
