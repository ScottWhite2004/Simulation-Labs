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

TEST(ShortestDistanceToLine, ClosestPointOnLine)
{
	Line line(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
	float distance = line.ShortestDistanceToPoint(glm::vec3(2, 3, 4));
	EXPECT_EQ(distance, 1.41f);
}

TEST(ShortestDistanceToLine, GeneralPointIsOnLine)
{
	Line line(glm::vec3(0, 0, 0), glm::vec3(1, 2, 3));
	float distance = line.ShortestDistanceToPoint(glm::vec3(3, 6, 9));
	EXPECT_EQ(distance, 0.0f);
}

TEST(ShortestDistanceToLine, VerticalLineCase)
{
	Line line(glm::vec3(2, 2, 0), glm::vec3(0, 0, 1));
	float distance = line.ShortestDistanceToPoint(glm::vec3(4, 5, 3));
	EXPECT_EQ(distance, 3.61f);
}

TEST(ShortestDistanceToLine, HorizontalLineCase)
{
	Line line(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0));
	float distance = line.ShortestDistanceToPoint(glm::vec3(3, 4, 5));
	EXPECT_EQ(distance, 6.40f);
}

TEST(ShortestDistanceToLine, DiagonalLineCase)
{
	Line line(glm::vec3(1, 1, 1), glm::vec3(1, -1, 1));
	float distance = line.ShortestDistanceToPoint(glm::vec3(2, 5, 3));
	EXPECT_EQ(distance, 4.55f);
}

TEST(SphereLineIntersection, NoIntersectionCentreAtOrigin)
{
	Line line(glm::vec3(5, 5, 5), glm::vec3(1, 0, 0));
	Sphere sphere(glm::vec3(0, 0, 0), 3);
	EXPECT_FALSE(sphere.Intersects(line));
}

TEST(SphereLineIntersection, IntersectsSphereCentreAtOrigin)
{
	Line line(glm::vec3(10, 0, 0), glm::vec3(-1, 0, 0));
	Sphere sphere(glm::vec3(10, 0, 0), 5);
	EXPECT_TRUE(sphere.Intersects(line));
}

TEST(SphereLineIntersection, IntersectsSphereLineStartInSphere)
{
	Line line(glm::vec3(3, 2, 2), glm::vec3(1, 0, 0));
	Sphere sphere(glm::vec3(2, 2, 2), 5);
	EXPECT_TRUE(sphere.Intersects(line));
}

TEST(SphereLineIntersection, IntersectsSphereLinePassesThroughSphereCentre)
{
	Line line(glm::vec3(-5, 0, 0), glm::vec3(1, 0, 0));
	Sphere sphere(glm::vec3(0, 0, 0), 3);
	EXPECT_TRUE(sphere.Intersects(line));
}

TEST(ShortestDistanceToPlane, PointAbovePlane)
{
	Plane plane(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
	glm::vec3 point(2, 3, 5);
	float distance = plane.ShortestDistanceToPoint(point);
	EXPECT_EQ(distance, 5.0f);
}

TEST(ShortestDistanceToPlane, PointBelowPlane)
{
	Plane plane(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
	glm::vec3 point(2, 3, -4);
	float distance = plane.ShortestDistanceToPoint(point);
	EXPECT_EQ(distance, 4.0f);
}

TEST(ShortestDistanceToPlane, PointOnPlane)
{
	Plane plane(glm::vec3(1, 1, 1),  1/sqrtf(3) * glm::vec3(1, 1, 1));
	glm::vec3 point(0, 2, 1);
	float distance = plane.ShortestDistanceToPoint(point);
	EXPECT_EQ(distance, 0.0f);
}

TEST(ShortestDistanceToPlane, PointCloseToPlane)
{
	Plane plane(glm::vec3(0, 0, 0), 1/sqrtf(2) * glm::vec3(1, 1, 0));
	glm::vec3 point(1, 1, 1);
	float distance = plane.ShortestDistanceToPoint(point);
	EXPECT_EQ(distance, 1.41f);
}

TEST(ShortestDistanceToPlane, PointWithNegativeCoordinates)
{
	Plane plane(glm::vec3(-2, -2, -2), 1/sqrtf(3) * glm::vec3(1, 1, 1));
	glm::vec3 point(-1, -1, -1);
	float distance = plane.ShortestDistanceToPoint(point);
	EXPECT_EQ(distance, 1.73f);
}

TEST(ShortestDistanceToPlane, PointAlongNormalVectorDirection)
{
	Plane plane(glm::vec3(0, 0, 0), 1/sqrtf(2) * glm::vec3(1, 1, 0));
	glm::vec3 point(1, 1, 0);
	float distance = plane.ShortestDistanceToPoint(point);
	EXPECT_EQ(distance, 1.41f);
}

TEST(ShortestDistanceToPlane, PointNearPlaneInRandomDirection)
{
	Plane plane(glm::vec3(0, 0, 0),  1/sqrtf(2) * glm::vec3(1, -1, 0));
	glm::vec3 point(1, 2, 3);
	float distance = plane.ShortestDistanceToPoint(point);
	EXPECT_EQ(distance, 0.71f);
}

TEST(SpherePlaneCollision, NoIntersectionSphereAbovePlane)
{
	Plane plane(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1)); 
	Sphere sphere(glm::vec3(0, 0, 5), 4);               
	EXPECT_FALSE(sphere.CollidesWith(plane));
}

TEST(SpherePlaneCollision, TangentTouchingSphereAbovePlane)
{
	Plane plane(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1)); 
	Sphere sphere(glm::vec3(0, 0, 5), 5);               
	EXPECT_TRUE(sphere.CollidesWith(plane));
}

TEST(SpherePlaneCollision, IntersectingSphereAbovePlane)
{
	Plane plane(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1)); 
	Sphere sphere(glm::vec3(0, 0, 3), 5);               
	EXPECT_TRUE(sphere.CollidesWith(plane));
}

TEST(SpherePlaneCollision, CentreOnPlane)
{
	Plane plane(glm::vec3(1, 1, 1), 1 / sqrtf(3) * glm::vec3(1, 1, 1));
	Sphere sphere(glm::vec3(0, 2, 1), 1); 
	EXPECT_TRUE(sphere.CollidesWith(plane));
}

TEST(SpherePlaneCollision, NoIntersectionSphereBelowPlane)
{
	Plane plane(glm::vec3(0, 0, 1), glm::vec3(0, 0, 1)); 
	Sphere sphere(glm::vec3(0, 0, -5), 5);               
	EXPECT_FALSE(sphere.CollidesWith(plane));
}

TEST(SpherePlaneCollision, TangentTouchingSphereBelowPlane)
{
	Plane plane(glm::vec3(0, 0, 1), glm::vec3(0, 0, 1));
	Sphere sphere(glm::vec3(0, 0, -4), 5);
	EXPECT_TRUE(sphere.CollidesWith(plane));
}

TEST(SpherePlaneCollision, IntersectingWithSkewedPlaneAdjustedRadius)
{
	Plane plane(glm::vec3(0, 0, 0), 1 / sqrtf(2) * glm::vec3(1, 1, 0));
	Sphere sphere(glm::vec3(1, 1, 0), 1.5f);
	EXPECT_TRUE(sphere.CollidesWith(plane));
}

TEST(SpherePlaneCollision, NoIntersectionWithSkewedPlaneAdjustedRadius)
{
	Plane plane(glm::vec3(0, 0, 0), 1 / sqrtf(2) * glm::vec3(1, -1, 0)); 
	Sphere sphere(glm::vec3(1, 2, 3), 0.5f); 
	EXPECT_FALSE(sphere.CollidesWith(plane));
}

TEST(SpherePlaneCollision, IntersectsWithSkewedPlaneAdjustedRadius)
{
	Plane plane(glm::vec3(0, 0, 0), 1 / sqrtf(2) * glm::vec3(1, -1, 0)); 
	Sphere sphere(glm::vec3(1, 2, 3), 1.0f); 
	EXPECT_TRUE(sphere.CollidesWith(plane));
}

TEST(SpherePlaneCollision, NonNormalizedPlaneNormalHandled)
{
	Plane plane(glm::vec3(0, 0, 0), glm::vec3(0, 0, 10)); 
	Sphere sphere(glm::vec3(0, 0, 0.9f), 1.0f);          
	EXPECT_TRUE(sphere.CollidesWith(plane));
}

TEST(SpherePlaneCollision, LargeRadiusAlwaysCollidesWithPlane)
{
	Plane plane(glm::vec3(10, 10, 10), glm::vec3(0, 0, 1));
	Sphere sphere(glm::vec3(-100, -100, -100), 1000.0f);
	EXPECT_TRUE(sphere.CollidesWith(plane));
}








