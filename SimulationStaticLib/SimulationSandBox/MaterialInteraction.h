#pragma once
#include <string>
struct MaterialInteraction
{
	std::string material_a;
	std::string material_b;
	float restitution;
	float static_friction;
	float dynamic_friction;
};

