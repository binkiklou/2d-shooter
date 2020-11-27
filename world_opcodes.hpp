#pragma once

// 0 - Opcode that should be called
// 1 - Pointer to server the called
#define WORLD_GET_OBJECTS 0

// Calls specified server with specified
// operation, when the world is changed.
// 0 - Opcode that should be called
// 1 - Pointer to server that called
#define WORLD_CALLBACK_IF_CHANGED 1

// 0 - Opcode that should be called
// 1 - Pointer to server that called
#define WORLD_CALLBACK_ON_TICK 2

// Pushes object to object vector
// 0 - Pointer to object
#define WORLD_PUSH_OBJECT 3

// Moves object in the world
// 0 - Object ID
// 1 - Pointer to Vector2 of direction
#define WORLD_PHYSICS_MOVE_OBJECT 10

// 0 - Object ID
// 1 - Pointer to float for angle
#define WORLD_PHYSICS_ROTATE_OBJECT 11

#define WORLD_QUIT 255