#pragma once

// 0 - Opcode that should be called
// 1 - Pointer to server the called
#define WORLD_GET_OBJECTS 0

// Calls specified server with specified
// operation, when the world is changed.
// Opcode that should be called
// 1 - Pointer to server that called
#define WORLD_CALLBACK_IF_CHANGED 1

// Pushes object to object vector
// 0 - Pointer to object
#define WORLD_PUSH_OBJECT 2