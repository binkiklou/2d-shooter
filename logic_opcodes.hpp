#pragma once

// 0 - Ptr to script
#define LOGIC_PUSH_SCRIPT 0

// "When gives you a empty pointer to the world
// Make it point to the main scene."
// 0 - Empty ptr, to world
#define LOGIC_GET_WORLD_PTR 1

// Forces the logic core to update
// the scripts. Should be called by
// the world, every tick.
#define LOGIC_UPDATE_SCRIPTS 2