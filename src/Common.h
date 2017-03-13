/*--------------------------------------------------------------------------------------------
-- HEADER FILE: Common.h
--
-- FUNCTIONS:

--
-- DATE:        March 12, 2017
--
-- DESIGNER:    Fred Yang, Robert Arendac
--
-- PROGRAMMER:  Fred Yang, Robert Arendac
--
-- NOTES:
-- This header file includes the common macro definitions and function 
-- prototypes for the project.
---------------------------------------------------------------------------------------------*/

#ifndef COMMON_H
#define COMMON_H
#include <stdio.h>

#define PI 3.14159265

// screen width/height
#define SCREEN_W    2000
#define SCREEN_H    2000

// The size of a tile/node and how large each zombie step is
#define TILE_SIZE   50
#define STEP_SPAN   50

// 8 possible directions
#define DIR_CAP     8
#define DIR_R       0
#define DIR_RD      1
#define DIR_D       2
#define DIR_LD      3
#define DIR_L       4
#define DIR_LU      5
#define DIR_U       6
#define DIR_RU      7

// Cardinal directions for setting angles
#define NORTH 0
#define NORTHEAST 45
#define EAST 90
#define SOUTHEAST 135
#define SOUTH 180
#define SOUTHWEST 225
#define WEST 270
#define NORTHWEST 315

#endif
