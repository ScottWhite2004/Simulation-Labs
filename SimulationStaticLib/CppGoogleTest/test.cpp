#include "pch.h"
#include "Sphere.h"
#include "PhysicsObject.h"
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

// ------------------------------------------------------------
// Helpers for PhysicsObject tests
// ------------------------------------------------------------
static PhysicsObject CreateSphereBody(const glm::vec3& position,
	const glm::vec3& velocity,
	float radius,
	float mass = 1.0f)
{
	Sphere sphereCollider(position, radius);
	PhysicsObject body(position, glm::vec3(0.0f), velocity, sphereCollider, mass);
	return body;
}

// ------------------------------------------------------------
// Sphere–sphere collision resolution tests (direct resolver)
// ------------------------------------------------------------

TEST(PhysicsResolution, HeadOnEqualMass_VelocitiesSwapAlongNormal)
{
	// Two equal?mass spheres on x?axis, moving toward each other
	PhysicsObject a = CreateSphereBody(
		glm::vec3(-1.0f, 0.0f, 0.0f),   // position
		glm::vec3(1.0f, 0.0f, 0.0f),   // velocity
		1.0f);                          // radius

	PhysicsObject b = CreateSphereBody(
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		1.0f);

	// Ensure spheres overlap (so collision is valid)
	ASSERT_TRUE(a.GetSphere().CollidesWith(b.GetSphere()));

	// Act
	a.ResolveSphereSphereCollision(b);

	// For equal masses and elastic collision, velocities swap
	EXPECT_EQ(a.getVel(), glm::vec3(-1.0f, 0.0f, 0.0f));
	EXPECT_EQ(b.getVel(), glm::vec3(1.0f, 0.0f, 0.0f));
}

TEST(PhysicsResolution, OneMovingOneStationary_HeadOn)
{
	// Moving sphere A toward stationary B
	PhysicsObject a = CreateSphereBody(
		glm::vec3(-2.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 0.0f, 0.0f),
		1.5f);                          // radius

	PhysicsObject b = CreateSphereBody(
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		1.5f);

	ASSERT_TRUE(a.GetSphere().CollidesWith(b.GetSphere()));

	// Act
	a.ResolveSphereSphereCollision(b);

	// After elastic equal?mass head?on collision:
	// A stops, B takes A's original velocity
	EXPECT_EQ(a.getVel(), glm::vec3(0.0f, 0.0f, 0.0f));
	EXPECT_EQ(b.getVel(), glm::vec3(2.0f, 0.0f, 0.0f));
}

TEST(PhysicsResolution, GlancingCollision_NormalComponentSwapped_TangentPreserved)
{
	// Line of centers along +x, tangent along +y
	// A moves down?right, B moves up?left
	PhysicsObject a = CreateSphereBody(
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, -1.0f, 0.0f),
		1.0f);

	PhysicsObject b = CreateSphereBody(
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 1.0f, 0.0f),
		1.0f);

	ASSERT_TRUE(a.GetSphere().CollidesWith(b.GetSphere()));

	glm::vec3 n(1.0f, 0.0f, 0.0f); // collision normal (line of centers)

	// Pre?collision velocities
	glm::vec3 va0 = a.getVel();
	glm::vec3 vb0 = b.getVel();

	float vaN0 = glm::dot(va0, n);
	float vbN0 = glm::dot(vb0, n);

	glm::vec3 vaT0 = va0 - vaN0 * n;
	glm::vec3 vbT0 = vb0 - vbN0 * n;

	// Act
	a.ResolveSphereSphereCollision(b);

	glm::vec3 va1 = a.getVel();
	glm::vec3 vb1 = b.getVel();

	float vaN1 = glm::dot(va1, n);
	float vbN1 = glm::dot(vb1, n);

	glm::vec3 vaT1 = va1 - vaN1 * n;
	glm::vec3 vbT1 = vb1 - vbN1 * n;

	// Tangential components (perpendicular to normal) unchanged
	EXPECT_EQ(vaT1, vaT0);
	EXPECT_EQ(vbT1, vbT0);

	// Normal components swapped for equal mass, elastic collision
	EXPECT_FLOAT_EQ(vaN1, vbN0);
	EXPECT_FLOAT_EQ(vbN1, vaN0);
}

TEST(PhysicsResolution, NoCollisionWhenSeparatingAlongNormal)
{
	// Spheres overlapping but moving away from each other along normal
	PhysicsObject a = CreateSphereBody(
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),   // moving left
		1.5f);

	PhysicsObject b = CreateSphereBody(
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),   // moving right
		1.5f);

	ASSERT_TRUE(a.GetSphere().CollidesWith(b.GetSphere()));

	glm::vec3 vaBefore = a.getVel();
	glm::vec3 vbBefore = b.getVel();

	// Relative normal speed > 0 ? resolver should early?out and do nothing
	a.ResolveSphereSphereCollision(b);

	EXPECT_EQ(a.getVel(), vaBefore);
	EXPECT_EQ(b.getVel(), vbBefore);
}

TEST(PhysicsResolution, DegenerateCase_CoincidentCenters_PicksArbitraryNormal)
{
	// Centres coincide; velocities head?on along some axis
	PhysicsObject a = CreateSphereBody(
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		1.0f);

	PhysicsObject b = CreateSphereBody(
		glm::vec3(0.0f, 0.0f, 0.0f),    // same centre
		glm::vec3(-1.0f, 0.0f, 0.0f),
		1.0f);

	ASSERT_TRUE(a.GetSphere().CollidesWith(b.GetSphere()));

	// Act
	a.ResolveSphereSphereCollision(b);

	// Even with coincident centres, velocities should still swap along
	// the arbitrary normal chosen in the implementation (1,0,0)
	EXPECT_EQ(a.getVel(), glm::vec3(-1.0f, 0.0f, 0.0f));
	EXPECT_EQ(b.getVel(), glm::vec3(1.0f, 0.0f, 0.0f));
}

// ------------------------------------------------------------
// Sphere–sphere collision resolution tests (different masses)
// ------------------------------------------------------------

static void Compute1DElasticCollision(
	float m1, float m2,
	float u1, float u2,
	float& v1, float& v2)
{
	// v1 = (m1 - m2)/(m1 + m2) * u1 + (2 m2)/(m1 + m2) * u2
	// v2 = (2 m1)/(m1 + m2) * u1 + (m2 - m1)/(m1 + m2) * u2
	const float denom = m1 + m2;
	v1 = ((m1 - m2) / denom) * u1 + (2.0f * m2 / denom) * u2;
	v2 = (2.0f * m1 / denom) * u1 + ((m2 - m1) / denom) * u2;
}

TEST(PhysicsResolution_DifferentMass, HeadOn_HeavyHitsLight)
{
	// Heavy sphere A (m = 2) moving right, light sphere B (m = 1) moving left.
	const float m1 = 2.0f;
	const float m2 = 1.0f;
	const float u1 = 1.0f;   // A along +x
	const float u2 = -1.0f;  // B along -x

	PhysicsObject a = CreateSphereBody(
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(u1, 0.0f, 0.0f),
		1.0f,
		m1);

	PhysicsObject b = CreateSphereBody(
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(u2, 0.0f, 0.0f),
		1.0f,
		m2);

	ASSERT_TRUE(a.GetSphere().CollidesWith(b.GetSphere()));

	float v1Expected = 0.0f;
	float v2Expected = 0.0f;
	Compute1DElasticCollision(m1, m2, u1, u2, v1Expected, v2Expected);

	// Act
	a.ResolveSphereSphereCollision(b);

	EXPECT_FLOAT_EQ(a.getVel().x, v1Expected);
	EXPECT_FLOAT_EQ(b.getVel().x, v2Expected);
	EXPECT_FLOAT_EQ(a.getVel().y, 0.0f);
	EXPECT_FLOAT_EQ(b.getVel().y, 0.0f);
	EXPECT_FLOAT_EQ(a.getVel().z, 0.0f);
	EXPECT_FLOAT_EQ(b.getVel().z, 0.0f);
}

TEST(PhysicsResolution_DifferentMass, HeadOn_LightHitsHeavy_StationaryHeavy)
{
	// Light sphere A (m = 1) moving toward heavy stationary B (m = 3)
	const float m1 = 1.0f;
	const float m2 = 3.0f;
	const float u1 = 2.0f;   // A along +x
	const float u2 = 0.0f;   // B at rest

	PhysicsObject a = CreateSphereBody(
		glm::vec3(-2.0f, 0.0f, 0.0f),
		glm::vec3(u1, 0.0f, 0.0f),
		1.0f,
		m1);

	PhysicsObject b = CreateSphereBody(
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(u2, 0.0f, 0.0f),
		1.0f,
		m2);

	ASSERT_TRUE(a.GetSphere().CollidesWith(b.GetSphere()));

	float v1Expected = 0.0f;
	float v2Expected = 0.0f;
	Compute1DElasticCollision(m1, m2, u1, u2, v1Expected, v2Expected);

	// Act
	a.ResolveSphereSphereCollision(b);

	EXPECT_FLOAT_EQ(a.getVel().x, v1Expected);  // expected -1.0
	EXPECT_FLOAT_EQ(b.getVel().x, v2Expected);  // expected  1.0
	EXPECT_FLOAT_EQ(a.getVel().y, 0.0f);
	EXPECT_FLOAT_EQ(b.getVel().y, 0.0f);
	EXPECT_FLOAT_EQ(a.getVel().z, 0.0f);
	EXPECT_FLOAT_EQ(b.getVel().z, 0.0f);
}

TEST(PhysicsResolution_DifferentMass, GlancingCollision_NormalUsesMass_TangentPreserved)
{
	// Different masses, with tangential components.
	// Normal is x?axis, tangential part is in y.
	const float m1 = 1.0f;  // light
	const float m2 = 3.0f;  // heavy

	glm::vec3 n(1.0f, 0.0f, 0.0f);

	PhysicsObject a = CreateSphereBody(
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, -1.0f, 0.0f),  // u1n = 1, tangent = (-1,0)
		1.0f,
		m1);

	PhysicsObject b = CreateSphereBody(
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f, 0.5f, 0.0f),  // u2n = -0.5, tangent = (0.5,0)
		1.0f,
		m2);

	ASSERT_TRUE(a.GetSphere().CollidesWith(b.GetSphere()));

	glm::vec3 va0 = a.getVel();
	glm::vec3 vb0 = b.getVel();

	const float u1n = glm::dot(va0, n);
	const float u2n = glm::dot(vb0, n);

	const glm::vec3 vaT0 = va0 - u1n * n;
	const glm::vec3 vbT0 = vb0 - u2n * n;

	float v1nExpected = 0.0f;
	float v2nExpected = 0.0f;
	Compute1DElasticCollision(m1, m2, u1n, u2n, v1nExpected, v2nExpected);

	// Act
	a.ResolveSphereSphereCollision(b);

	glm::vec3 va1 = a.getVel();
	glm::vec3 vb1 = b.getVel();

	const float v1n = glm::dot(va1, n);
	const float v2n = glm::dot(vb1, n);
	const glm::vec3 vaT1 = va1 - v1n * n;
	const glm::vec3 vbT1 = vb1 - v2n * n;

	// Tangential components must remain unchanged
	EXPECT_FLOAT_EQ(vaT1.x, vaT0.x);
	EXPECT_FLOAT_EQ(vaT1.y, vaT0.y);
	EXPECT_FLOAT_EQ(vaT1.z, vaT0.z);

	EXPECT_FLOAT_EQ(vbT1.x, vbT0.x);
	EXPECT_FLOAT_EQ(vbT1.y, vbT0.y);
	EXPECT_FLOAT_EQ(vbT1.z, vbT0.z);

	// Normal components must match the different?mass formula
	EXPECT_FLOAT_EQ(v1n, v1nExpected);
	EXPECT_FLOAT_EQ(v2n, v2nExpected);
}








