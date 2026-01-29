#include "pch.h"
#include "Sphere.h"
#include <glm/glm.hpp>

TEST(SphereSphereCollision, NoIntersectionCentreAtOrigin)
{
	Sphere sphereA(glm::vec3(0, 0, 0), 1);
	Sphere sphereB(glm::vec3(5, 0, 0), 1);

	EXPECT_FALSE(sphereA.CollidesWith(sphereB));
}

TEST(SphereSphereCollision, NoIntersectionOffsetOrigin)
{
	Sphere sphereA(glm::vec3(3, 3, 3), 2);
	Sphere sphereB(glm::vec3(10, 10, 10), 2);

	EXPECT_FALSE(sphereA.CollidesWith(sphereB));
}

TEST(SphereSphereCollision, OverlappingSphereCentreAtOrigin)
{
	Sphere sphereA(glm::vec3(0, 0, 0), 2);
	Sphere sphereB(glm::vec3(2, 0, 0), 2);

	EXPECT_TRUE(sphereA.CollidesWith(sphereB));
}

TEST(SphereSphereCollision, OverlappingSphereOffsetOrigin)
{
	Sphere sphereA(glm::vec3(5, 5, 5), 3);
	Sphere sphereB(glm::vec3(8, 5, 5), 3);

	EXPECT_TRUE(sphereA.CollidesWith(sphereB));
}

TEST(SphereSphereCollision, FullyContainedCentreAtOrigin)
{
	Sphere sphereA(glm::vec3(0, 0, 0), 3);
	Sphere sphereB(glm::vec3(1, 0, 0), 1);

	EXPECT_TRUE(sphereA.CollidesWith(sphereB));
}

TEST(SphereSphereCollision, FullyContainedOffsetCentre)
{
	Sphere sphereA(glm::vec3(6, 6, 6), 5);
	Sphere sphereB(glm::vec3(7, 6, 6), 2);

	EXPECT_TRUE(sphereA.CollidesWith(sphereB));
}




