#pragma once
#include "Waypoint.h"
#include <vector>
enum class EasingType
{
	LINEAR,
	SMOOTHSTEP
};

enum class PathMode
{
	STOP,
	LOOP,
	REVERSE
};

class AnimatedObject
{
	std::vector<Waypoint> waypoints;
	float total_duration;
	EasingType easing;
	PathMode path_mode;
};

