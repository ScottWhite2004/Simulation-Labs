#pragma once
#include "PhysicsState.h"

enum ObjectOwnerType
{
	ONE,
	TWO,
	THREE,
	FOUR
};

class SimulatedObject
{
	PhysicsState initial_state;
	ObjectOwnerType owner;
};

