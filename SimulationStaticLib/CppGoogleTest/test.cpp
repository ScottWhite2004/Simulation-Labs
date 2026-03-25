#include "pch.h"
#include "SphereCollider.h"
#include "PhysicsObject.h"
#include <glm/glm.hpp>



TEST(SphereSphereCollision, NoIntersectionCentreAtOrigin)
{
	SphereCollider sphereA(glm::vec3(0, 0, 0), 1);
	SphereCollider sphereB(glm::vec3(5, 0, 0), 1);

	EXPECT_FALSE(sphereA.CollidesWith(sphereB));
}

TEST(SphereSphereCollision, NoIntersectionOffsetOrigin)
{
	SphereCollider sphereA(glm::vec3(3, 3, 3), 2);
	SphereCollider sphereB(glm::vec3(10, 10, 10), 2);

	EXPECT_FALSE(sphereA.CollidesWith(sphereB));
}

TEST(SphereSphereCollision, OverlappingSphereCentreAtOrigin)
{
	SphereCollider sphereA(glm::vec3(0, 0, 0), 2);
	SphereCollider sphereB(glm::vec3(2, 0, 0), 2);

	EXPECT_TRUE(sphereA.CollidesWith(sphereB));
}

TEST(SphereSphereCollision, OverlappingSphereOffsetOrigin)
{
	SphereCollider sphereA(glm::vec3(5, 5, 5), 3);
	SphereCollider sphereB(glm::vec3(8, 5, 5), 3);

	EXPECT_TRUE(sphereA.CollidesWith(sphereB));
}

TEST(SphereSphereCollision, FullyContainedCentreAtOrigin)
{
	SphereCollider sphereA(glm::vec3(0, 0, 0), 3);
	SphereCollider sphereB(glm::vec3(1, 0, 0), 1);

	EXPECT_TRUE(sphereA.CollidesWith(sphereB));
}

TEST(SphereSphereCollision, FullyContainedOffsetCentre)
{
	SphereCollider sphereA(glm::vec3(6, 6, 6), 5);
	SphereCollider sphereB(glm::vec3(7, 6, 6), 2);

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
	SphereCollider sphere(glm::vec3(0, 0, 0), 3);
	EXPECT_FALSE(sphere.Intersects(line));
}

TEST(SphereLineIntersection, IntersectsSphereCentreAtOrigin)
{
	Line line(glm::vec3(10, 0, 0), glm::vec3(-1, 0, 0));
	SphereCollider sphere(glm::vec3(10, 0, 0), 5);
	EXPECT_TRUE(sphere.Intersects(line));
}

TEST(SphereLineIntersection, IntersectsSphereLineStartInSphere)
{
	Line line(glm::vec3(3, 2, 2), glm::vec3(1, 0, 0));
	SphereCollider sphere(glm::vec3(2, 2, 2), 5);
	EXPECT_TRUE(sphere.Intersects(line));
}

TEST(SphereLineIntersection, IntersectsSphereLinePassesThroughSphereCentre)
{
	Line line(glm::vec3(-5, 0, 0), glm::vec3(1, 0, 0));
	SphereCollider sphere(glm::vec3(0, 0, 0), 3);
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
	SphereCollider sphere(glm::vec3(0, 0, 5), 4);               
	EXPECT_FALSE(sphere.CollidesWith(plane));
}

TEST(SpherePlaneCollision, TangentTouchingSphereAbovePlane)
{
	Plane plane(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1)); 
	SphereCollider sphere(glm::vec3(0, 0, 5), 5);               
	EXPECT_TRUE(sphere.CollidesWith(plane));
}

TEST(SpherePlaneCollision, IntersectingSphereAbovePlane)
{
	Plane plane(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1)); 
	SphereCollider sphere(glm::vec3(0, 0, 3), 5);               
	EXPECT_TRUE(sphere.CollidesWith(plane));
}

TEST(SpherePlaneCollision, CentreOnPlane)
{
	Plane plane(glm::vec3(1, 1, 1), 1 / sqrtf(3) * glm::vec3(1, 1, 1));
	SphereCollider sphere(glm::vec3(0, 2, 1), 1); 
	EXPECT_TRUE(sphere.CollidesWith(plane));
}

TEST(SpherePlaneCollision, NoIntersectionSphereBelowPlane)
{
	Plane plane(glm::vec3(0, 0, 1), glm::vec3(0, 0, 1)); 
	SphereCollider sphere(glm::vec3(0, 0, -5), 5);               
	EXPECT_FALSE(sphere.CollidesWith(plane));
}

TEST(SpherePlaneCollision, TangentTouchingSphereBelowPlane)
{
	Plane plane(glm::vec3(0, 0, 1), glm::vec3(0, 0, 1));
	SphereCollider sphere(glm::vec3(0, 0, -4), 5);
	EXPECT_TRUE(sphere.CollidesWith(plane));
}

TEST(SpherePlaneCollision, IntersectingWithSkewedPlaneAdjustedRadius)
{
	Plane plane(glm::vec3(0, 0, 0), 1 / sqrtf(2) * glm::vec3(1, 1, 0));
	SphereCollider sphere(glm::vec3(1, 1, 0), 1.5f);
	EXPECT_TRUE(sphere.CollidesWith(plane));
}

TEST(SpherePlaneCollision, NoIntersectionWithSkewedPlaneAdjustedRadius)
{
	Plane plane(glm::vec3(0, 0, 0), 1 / sqrtf(2) * glm::vec3(1, -1, 0)); 
	SphereCollider sphere(glm::vec3(1, 2, 3), 0.5f); 
	EXPECT_FALSE(sphere.CollidesWith(plane));
}

TEST(SpherePlaneCollision, IntersectsWithSkewedPlaneAdjustedRadius)
{
	Plane plane(glm::vec3(0, 0, 0), 1 / sqrtf(2) * glm::vec3(1, -1, 0)); 
	SphereCollider sphere(glm::vec3(1, 2, 3), 1.0f); 
	EXPECT_TRUE(sphere.CollidesWith(plane));
}

TEST(SpherePlaneCollision, NonNormalizedPlaneNormalHandled)
{
	Plane plane(glm::vec3(0, 0, 0), glm::vec3(0, 0, 10)); 
	SphereCollider sphere(glm::vec3(0, 0, 0.9f), 1.0f);          
	EXPECT_TRUE(sphere.CollidesWith(plane));
}

TEST(SpherePlaneCollision, LargeRadiusAlwaysCollidesWithPlane)
{
	Plane plane(glm::vec3(10, 10, 10), glm::vec3(0, 0, 1));
	SphereCollider sphere(glm::vec3(-100, -100, -100), 1000.0f);
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
	SphereCollider sphereCollider(position, radius);
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

TEST(AngularDisplacement, 90DegreesXAxis)
{
	PhysicsObject obj;
	obj.SetOrientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	obj.addAngularDisplacement(glm::vec3(1.0f, 0.0f, 0.0f), glm::radians(90.0f));
	glm::quat q = obj.getOrientation();

	EXPECT_NEAR(q.w, 0.7071f, 0.001f);
	EXPECT_NEAR(q.x, 0.7071f, 0.001f);
	EXPECT_NEAR(q.y, 0.0f, 0.001f);
	EXPECT_NEAR(q.z, 0.0f, 0.001f);
}

TEST(AngularDisplacement, 90DegreesYAxis)
{
	PhysicsObject obj;
	obj.SetOrientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	obj.addAngularDisplacement(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(90.0f));
	glm::quat q = obj.getOrientation();

	EXPECT_NEAR(q.w, 0.7071f, 0.001f);
	EXPECT_NEAR(q.x, 0.0f, 0.001f);
	EXPECT_NEAR(q.y, 0.7071f, 0.001f);
	EXPECT_NEAR(q.z, 0.0f, 0.001f);
}

TEST(AngularDisplacement, 90DegreesZAxis)
{
	PhysicsObject obj;
	obj.SetOrientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	obj.addAngularDisplacement(glm::vec3(0.0f, 0.0f, 1.0f), glm::radians(90.0f));
	glm::quat q = obj.getOrientation();

	EXPECT_NEAR(q.w, 0.7071f, 0.001f);
	EXPECT_NEAR(q.x, 0.0f, 0.001f);
	EXPECT_NEAR(q.y, 0.0f, 0.001f);
	EXPECT_NEAR(q.z, 0.7071f, 0.001f);
}

TEST(AngularDisplacement, 180DegreesXAxis)
{
	PhysicsObject obj;
	obj.SetOrientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	obj.addAngularDisplacement(glm::vec3(1.0f, 0.0f, 0.0f), glm::radians(180.0f));
	glm::quat q = obj.getOrientation();

	EXPECT_NEAR(q.w, 0.0f, 0.001f);
	EXPECT_NEAR(q.x, 1.0f, 0.001f);
	EXPECT_NEAR(q.y, 0.0f, 0.001f);
	EXPECT_NEAR(q.z, 0.0f, 0.001f);
}

TEST(AngularDisplacement, 180DegreesYAxis)
{
	PhysicsObject obj;
	obj.SetOrientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	obj.addAngularDisplacement(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(180.0f));
	glm::quat q = obj.getOrientation();

	EXPECT_NEAR(q.w, 0.0f, 0.001f);
	EXPECT_NEAR(q.x, 0.0f, 0.001f);
	EXPECT_NEAR(q.y, 1.0f, 0.001f);
	EXPECT_NEAR(q.z, 0.0f, 0.001f);
}

TEST(AngularDisplacement, 180DegreesZAxis)
{
	PhysicsObject obj;
	obj.SetOrientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	obj.addAngularDisplacement(glm::vec3(0.0f, 0.0f, 1.0f), glm::radians(180.0f));
	glm::quat q = obj.getOrientation();

	EXPECT_NEAR(q.w, 0.0f, 0.001f);
	EXPECT_NEAR(q.x, 0.0f, 0.001f);
	EXPECT_NEAR(q.y, 0.0f, 0.001f);
	EXPECT_NEAR(q.z, 1.0f, 0.001f);
}

TEST(AngularDisplacement, 270DegreesXAxis)
{
	PhysicsObject obj;
	obj.SetOrientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	obj.addAngularDisplacement(glm::vec3(1.0f, 0.0f, 0.0f), glm::radians(270.0f));
	glm::quat q = obj.getOrientation();

	EXPECT_NEAR(q.w, -0.7071f, 0.001f);
	EXPECT_NEAR(q.x, 0.7071f, 0.001f);
	EXPECT_NEAR(q.y, 0.0f, 0.001f);
	EXPECT_NEAR(q.z, 0.0f, 0.001f);
}

TEST(AngularDisplacement, 270DegreesYAxis)
{
	PhysicsObject obj;
	obj.SetOrientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	obj.addAngularDisplacement(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(270.0f));
	glm::quat q = obj.getOrientation();

	EXPECT_NEAR(q.w, -0.7071f, 0.001f);
	EXPECT_NEAR(q.x, 0.0f, 0.001f);
	EXPECT_NEAR(q.y, 0.7071f, 0.001f);
	EXPECT_NEAR(q.z, 0.0f, 0.001f);
}

TEST(AngularDisplacement, 270DegreesZAxis)
{
	PhysicsObject obj;
	obj.SetOrientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	obj.addAngularDisplacement(glm::vec3(0.0f, 0.0f, 1.0f), glm::radians(270.0f));
	glm::quat q = obj.getOrientation();

	EXPECT_NEAR(q.w, -0.7071f, 0.001f);
	EXPECT_NEAR(q.x, 0.0f, 0.001f);
	EXPECT_NEAR(q.y, 0.0f, 0.001f);
	EXPECT_NEAR(q.z, 0.7071f, 0.001f);
}

TEST(AngularDisplacement, 360DegreesXAxis)
{
	PhysicsObject obj;
	obj.SetOrientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	obj.addAngularDisplacement(glm::vec3(1.0f, 0.0f, 0.0f), glm::radians(360.0f));
	glm::quat q = obj.getOrientation();

	EXPECT_NEAR(q.w, -1.0f, 0.001f);
	EXPECT_NEAR(q.x, 0.0f, 0.001f);
	EXPECT_NEAR(q.y, 0.0f, 0.001f);
	EXPECT_NEAR(q.z, 0.0f, 0.001f);
}

TEST(AngularDisplacement, 360DegreesYAxis)
{
	PhysicsObject obj;
	obj.SetOrientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	obj.addAngularDisplacement(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(360.0f));
	glm::quat q = obj.getOrientation();

	EXPECT_NEAR(q.w, -1.0f, 0.001f);
	EXPECT_NEAR(q.x, 0.0f, 0.001f);
	EXPECT_NEAR(q.y, 0.0f, 0.001f);
	EXPECT_NEAR(q.z, 0.0f, 0.001f);
}

TEST(AngularDisplacement, 360DegreesZAxis)
{
	PhysicsObject obj;
	obj.SetOrientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	obj.addAngularDisplacement(glm::vec3(0.0f, 0.0f, 1.0f), glm::radians(360.0f));
	glm::quat q = obj.getOrientation();

	EXPECT_NEAR(q.w, -1.0f, 0.001f);
	EXPECT_NEAR(q.x, 0.0f, 0.001f);
	EXPECT_NEAR(q.y, 0.0f, 0.001f);
	EXPECT_NEAR(q.z, 0.0f, 0.001f);
}

TEST(AngularDisplacement, 90DegreesXYZAxis)
{
	PhysicsObject obj;
	obj.SetOrientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	obj.addAngularDisplacement(glm::vec3(1.0f, 1.0f, 1.0f), glm::radians(90.0f));
	glm::quat q = obj.getOrientation();

	// Axis is normalized to (0.577, 0.577, 0.577)
	// sin(45) = 0.7071, cos(45) = 0.7071
	// Values for x,y,z will be 0.577 * 0.7071 = 0.4082
	EXPECT_NEAR(q.w, 0.7071f, 0.001f);
	EXPECT_NEAR(q.x, 0.4082f, 0.001f);
	EXPECT_NEAR(q.y, 0.4082f, 0.001f);
	EXPECT_NEAR(q.z, 0.4082f, 0.001f);
}

TEST(AngularDisplacement, 90DegreesXYAxis180DegreesZAxis)
{
	PhysicsObject obj;
	obj.SetOrientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));

	// 1. +90 degrees around XY
	obj.addAngularDisplacement(glm::vec3(1.0f, 1.0f, 0.0f), glm::radians(90.0f));
	// 2. +180 degrees around Z
	obj.addAngularDisplacement(glm::vec3(0.0f, 0.0f, 1.0f), glm::radians(180.0f));

	glm::quat q = obj.getOrientation();

	EXPECT_NEAR(q.w, 0.0f, 0.001f);
	EXPECT_NEAR(q.x, 0.5f, 0.001f);
	EXPECT_NEAR(q.y, -0.5f, 0.001f);
	EXPECT_NEAR(q.z, 0.7071f, 0.001f);
}

TEST(AngularDisplacement, 90DegreesXZAxis180DegreesYAxis)
{
	PhysicsObject obj;
	obj.SetOrientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	obj.addAngularDisplacement(glm::vec3(1.0f, 0.0f, 1.0f), glm::radians(90.0f));
	obj.addAngularDisplacement(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(180.0f));
	glm::quat q = obj.getOrientation();

	EXPECT_NEAR(q.w, 0.0f, 0.001f);
	EXPECT_NEAR(q.x, -0.5f, 0.001f);
	EXPECT_NEAR(q.y, 0.7071f, 0.001f);
	EXPECT_NEAR(q.z, 0.5f, 0.001f);
}

TEST(AngularDisplacement, 90DegreesYZAxis180DegreesXAxis)
{
	PhysicsObject obj;
	obj.SetOrientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	obj.addAngularDisplacement(glm::vec3(0.0f, 1.0f, 1.0f), glm::radians(90.0f));
	obj.addAngularDisplacement(glm::vec3(1.0f, 0.0f, 0.0f), glm::radians(180.0f));
	glm::quat q = obj.getOrientation();

	EXPECT_NEAR(q.w, 0.0f, 0.001f);
	EXPECT_NEAR(q.x, 0.7071f, 0.001f);
	EXPECT_NEAR(q.y, 0.5f, 0.001f);
	EXPECT_NEAR(q.z, -0.5f, 0.001f);
}

TEST(AngularVelocity, 90DegreesXAxis_1Second)
{
	PhysicsObject obj;
	obj.SetOrientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	obj.SetAngularVelocity(glm::vec3(glm::radians(90.0f), 0.0f, 0.0f));
	obj.IntegrateEuler(1.0f); // 1.0s timestep
	glm::quat q = obj.getOrientation();

	EXPECT_NEAR(q.w, 0.7071f, 0.001f);
	EXPECT_NEAR(q.x, 0.7071f, 0.001f);
	EXPECT_NEAR(q.y, 0.0f, 0.001f);
	EXPECT_NEAR(q.z, 0.0f, 0.001f);
}

TEST(AngularVelocity, 90DegreesYAxis_HalfSecond)
{
	PhysicsObject obj;
	obj.SetOrientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	// 180 deg/s * 0.5s = 90 deg total displacement
	obj.SetAngularVelocity(glm::vec3(0.0f, glm::radians(180.0f), 0.0f));
	obj.IntegrateEuler(0.5f);
	glm::quat q = obj.getOrientation();

	EXPECT_NEAR(q.w, 0.7071f, 0.001f);
	EXPECT_NEAR(q.x, 0.0f, 0.001f);
	EXPECT_NEAR(q.y, 0.7071f, 0.001f);
	EXPECT_NEAR(q.z, 0.0f, 0.001f);
}

TEST(AngularVelocity, 90DegreesZAxis_TwoSeconds)
{
	PhysicsObject obj;
	obj.SetOrientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	// 45 deg/s * 2.0s = 90 deg total displacement
	obj.SetAngularVelocity(glm::vec3(0.0f, 0.0f, glm::radians(45.0f)));
	obj.IntegrateSemiImplicitEuler(2.0f); // Test the other integrator too
	glm::quat q = obj.getOrientation();

	EXPECT_NEAR(q.w, 0.7071f, 0.001f);
	EXPECT_NEAR(q.x, 0.0f, 0.001f);
	EXPECT_NEAR(q.y, 0.0f, 0.001f);
	EXPECT_NEAR(q.z, 0.7071f, 0.001f);
}

TEST(AngularVelocity, 180DegreesXAxis_QuarterSecond)
{
	PhysicsObject obj;
	obj.SetOrientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	// 720 deg/s * 0.25s = 180 deg total displacement
	obj.SetAngularVelocity(glm::vec3(glm::radians(720.0f), 0.0f, 0.0f));
	obj.IntegrateEuler(0.25f);
	glm::quat q = obj.getOrientation();

	EXPECT_NEAR(q.w, 0.0f, 0.001f);
	EXPECT_NEAR(q.x, 1.0f, 0.001f);
	EXPECT_NEAR(q.y, 0.0f, 0.001f);
	EXPECT_NEAR(q.z, 0.0f, 0.001f);
}

TEST(AngularVelocity, 180DegreesYAxis_0_1Second)
{
	PhysicsObject obj;
	obj.SetOrientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	// 1800 deg/s * 0.1s = 180 deg total displacement
	obj.SetAngularVelocity(glm::vec3(0.0f, glm::radians(1800.0f), 0.0f));
	obj.IntegrateEuler(0.1f);
	glm::quat q = obj.getOrientation();

	EXPECT_NEAR(q.w, 0.0f, 0.001f);
	EXPECT_NEAR(q.x, 0.0f, 0.001f);
	EXPECT_NEAR(q.y, 1.0f, 0.001f);
	EXPECT_NEAR(q.z, 0.0f, 0.001f);
}

TEST(AngularVelocity, 270DegreesZAxis_SemiImplicit_1Second)
{
	PhysicsObject obj;
	obj.SetOrientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	obj.SetAngularVelocity(glm::vec3(0.0f, 0.0f, glm::radians(270.0f)));
	obj.IntegrateSemiImplicitEuler(1.0f);
	glm::quat q = obj.getOrientation();

	EXPECT_NEAR(q.w, -0.7071f, 0.001f);
	EXPECT_NEAR(q.x, 0.0f, 0.001f);
	EXPECT_NEAR(q.y, 0.0f, 0.001f);
	EXPECT_NEAR(q.z, 0.7071f, 0.001f);
}

TEST(AngularVelocity, 360DegreesXAxis_MultipleSteps)
{
	PhysicsObject obj;
	obj.SetOrientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	// 180 deg/s * 2 updates of 1.0s = 360 deg total displacement
	obj.SetAngularVelocity(glm::vec3(glm::radians(180.0f), 0.0f, 0.0f));
	obj.IntegrateEuler(1.0f);
	obj.IntegrateEuler(1.0f);
	glm::quat q = obj.getOrientation();

	EXPECT_NEAR(q.w, -1.0f, 0.001f);
	EXPECT_NEAR(q.x, 0.0f, 0.001f);
	EXPECT_NEAR(q.y, 0.0f, 0.001f);
	EXPECT_NEAR(q.z, 0.0f, 0.001f);
}

TEST(AngularVelocity, 90DegreesXYZAxis_2Seconds)
{
	PhysicsObject obj;
	obj.SetOrientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));

	// Normalize the axis first or calculate displacement directly
	glm::vec3 axis(1.0f, 1.0f, 1.0f);
	glm::vec3 normalizedAxis = glm::normalize(axis);

	// Velocity needs to be along normalized axis to get exact test overlap
	// 45 degrees/sec * 2 seconds = 90 Deg Total
	obj.SetAngularVelocity(normalizedAxis * glm::radians(45.0f));
	obj.IntegrateEuler(2.0f);

	glm::quat q = obj.getOrientation();

	EXPECT_NEAR(q.w, 0.7071f, 0.001f);
	EXPECT_NEAR(q.x, 0.4082f, 0.001f);
	EXPECT_NEAR(q.y, 0.4082f, 0.001f);
	EXPECT_NEAR(q.z, 0.4082f, 0.001f);
}

static void ExpectVec3Near(const glm::vec3& actual, const glm::vec3& expected, float tolerance = 0.001f)
{
	EXPECT_NEAR(actual.x, expected.x, tolerance);
	EXPECT_NEAR(actual.y, expected.y, tolerance);
	EXPECT_NEAR(actual.z, expected.z, tolerance);
}

TEST(TorqueAccumulation, MultipleTorquesInSingleStep_AccumulateIntoAngularVelocity)
{
	PhysicsObject obj;
	obj.SetOrientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	obj.SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));

	// Two torques that should add to +90 deg/s around Z.
	obj.addTorque(glm::vec3(0.0f, 0.0f, glm::radians(30.0f)));
	obj.addTorque(glm::vec3(0.0f, 0.0f, glm::radians(60.0f)));

	obj.IntegrateEuler(1.0f);

	// Expected angular velocity after one second: +90 deg/s around Z.
	ExpectVec3Near(obj.getAngularVel(), glm::vec3(0.0f, 0.0f, glm::radians(90.0f)));

	// Expected orientation after one second at +90 deg about Z.
	glm::quat q = obj.getOrientation();
	EXPECT_NEAR(q.w, 0.7071f, 0.001f);
	EXPECT_NEAR(q.x, 0.0f, 0.001f);
	EXPECT_NEAR(q.y, 0.0f, 0.001f);
	EXPECT_NEAR(q.z, 0.7071f, 0.001f);
}

TEST(TorqueAccumulation, OpposingTorquesInSingleStep_CancelOut)
{
	PhysicsObject obj;
	obj.SetOrientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	obj.SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));

	obj.addTorque(glm::vec3(0.0f, 0.0f, glm::radians(45.0f)));
	obj.addTorque(glm::vec3(0.0f, 0.0f, glm::radians(-45.0f)));

	obj.IntegrateEuler(1.0f);

	// Net torque should be zero.
	ExpectVec3Near(obj.getAngularVel(), glm::vec3(0.0f, 0.0f, 0.0f));

	glm::quat q = obj.getOrientation();
	EXPECT_NEAR(q.w, 1.0f, 0.001f);
	EXPECT_NEAR(q.x, 0.0f, 0.001f);
	EXPECT_NEAR(q.y, 0.0f, 0.001f);
	EXPECT_NEAR(q.z, 0.0f, 0.001f);
}

TEST(AngularVelocityAccumulation, RepeatedEqualTorqueAcrossSteps_AccumulatesAngularVelocity)
{
	PhysicsObject obj;
	obj.SetOrientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	obj.SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));

	// Step 1: add +90 deg/s torque-equivalent and integrate for 1s.
	obj.addTorque(glm::vec3(0.0f, 0.0f, glm::radians(90.0f)));
	obj.IntegrateEuler(1.0f);

	// Step 2: apply the same torque again and integrate for 1s.
	obj.addTorque(glm::vec3(0.0f, 0.0f, glm::radians(90.0f)));
	obj.IntegrateEuler(1.0f);

	// Angular velocity should have accumulated to +180 deg/s about Z.
	ExpectVec3Near(obj.getAngularVel(), glm::vec3(0.0f, 0.0f, glm::radians(180.0f)));
}

TEST(ForceAtPoint, ForceThroughCentreOfMass_ChangesLinearVelocityOnly)
{
	PhysicsObject obj = CreateSphereBody(
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		1.0f,
		2.0f);

	obj.SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
	obj.addForceAtPoint(glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	obj.IntegrateEuler(1.0f);

	EXPECT_GT(obj.getVel().x, 0.0f);
	EXPECT_NEAR(obj.getAngularVel().x, 0.0f, 0.001f);
	EXPECT_NEAR(obj.getAngularVel().y, 0.0f, 0.001f);
	EXPECT_NEAR(obj.getAngularVel().z, 0.0f, 0.001f);
}

TEST(ForceAtPoint, ForceOffCentre_ChangesLinearAndAngularVelocity)
{
	PhysicsObject obj = CreateSphereBody(
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		1.0f,
		2.0f);

	obj.SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
	obj.addForceAtPoint(glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	obj.IntegrateEuler(1.0f);

	EXPECT_GT(obj.getVel().x, 0.0f);
	EXPECT_GT(glm::length(obj.getAngularVel()), 0.0001f);
}

TEST(ForceAtPoint, EqualOppositeForcesAtDifferentPoints_CreatePureTorque)
{
	PhysicsObject obj = CreateSphereBody(
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		1.0f,
		2.0f);

	obj.SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));

	// Net force = 0, net torque != 0
	obj.addForceAtPoint(glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	obj.addForceAtPoint(glm::vec3(-10.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	obj.IntegrateEuler(1.0f);

	EXPECT_NEAR(obj.getVel().x, 0.0f, 0.001f);
	EXPECT_NEAR(obj.getVel().y, 0.0f, 0.001f);
	EXPECT_NEAR(obj.getVel().z, 0.0f, 0.001f);
	EXPECT_GT(glm::length(obj.getAngularVel()), 0.0001f);
}

TEST(ForceAtPoint, SameDirectionForcesAtMirroredPoints_CreatePureTranslation)
{
	PhysicsObject obj = CreateSphereBody(
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		1.0f,
		2.0f);

	obj.SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));

	// Net force != 0, torques cancel
	obj.addForceAtPoint(glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	obj.addForceAtPoint(glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	obj.IntegrateEuler(1.0f);

	EXPECT_GT(obj.getVel().x, 0.0f);
	EXPECT_NEAR(obj.getAngularVel().x, 0.0f, 0.001f);
	EXPECT_NEAR(obj.getAngularVel().y, 0.0f, 0.001f);
	EXPECT_NEAR(obj.getAngularVel().z, 0.0f, 0.001f);
}

static float CalculateSphereInertia(float mass, float radius)
{
	return (2.0f / 5.0f) * mass * radius * radius;
}

TEST(RotationalInertia, EqualTorque_DoubleMass_HalfAngularAcceleration)
{
	// For a sphere: I = (2/5) * m * r²
	// If we keep radius constant and double mass, inertia doubles
	// Angular acceleration ? = ? / I, so doubling I halves ?

	const float radius = 1.0f;
	const float mass1 = 1.0f;
	const float mass2 = 2.0f;
	const glm::vec3 torque(0.0f, 0.0f, glm::radians(90.0f));

	PhysicsObject obj1 = CreateSphereBody(glm::vec3(0.0f), glm::vec3(0.0f), radius, mass1);
	PhysicsObject obj2 = CreateSphereBody(glm::vec3(0.0f), glm::vec3(0.0f), radius, mass2);

	obj1.SetAngularVelocity(glm::vec3(0.0f));
	obj2.SetAngularVelocity(glm::vec3(0.0f));

	// Apply same torque to both
	obj1.addTorque(torque);
	obj2.addTorque(torque);

	// Integrate for 1 second
	obj1.IntegrateEuler(1.0f);
	obj2.IntegrateEuler(1.0f);

	// With proper inertia implementation:
	// obj1 should have 2x the angular velocity of obj2
	float I1 = CalculateSphereInertia(mass1, radius);
	float I2 = CalculateSphereInertia(mass2, radius);

	float expectedRatio = I2 / I1; // Should be 2.0

	float angVel1 = glm::length(obj1.getAngularVel());
	float angVel2 = glm::length(obj2.getAngularVel());

	// Note: This test will currently fail because PhysicsObject uses unit inertia
	// When inertia is properly implemented, uncomment these assertions:
	 EXPECT_FLOAT_EQ(angVel1 / angVel2, expectedRatio);

	// For now, verify the physics expectation:
	EXPECT_FLOAT_EQ(expectedRatio, 2.0f);
}

TEST(RotationalInertia, DoubleTorque_DoubleMass_SameAngularVelocity)
{
	// If we double both mass and torque, angular acceleration stays the same
	// ? = ? / I, so (2?) / (2I) = ? / I

	const float radius = 1.0f;
	const float mass1 = 1.0f;
	const float mass2 = 2.0f;
	const glm::vec3 torque1(0.0f, 0.0f, glm::radians(45.0f));
	const glm::vec3 torque2 = torque1 * 2.0f;

	PhysicsObject obj1 = CreateSphereBody(glm::vec3(0.0f), glm::vec3(0.0f), radius, mass1);
	PhysicsObject obj2 = CreateSphereBody(glm::vec3(0.0f), glm::vec3(0.0f), radius, mass2);

	obj1.SetAngularVelocity(glm::vec3(0.0f));
	obj2.SetAngularVelocity(glm::vec3(0.0f));

	obj1.addTorque(torque1);
	obj2.addTorque(torque2);

	obj1.IntegrateEuler(1.0f);
	obj2.IntegrateEuler(1.0f);

	float I1 = CalculateSphereInertia(mass1, radius);
	float I2 = CalculateSphereInertia(mass2, radius);

	// When properly implemented, these should be equal:
	 EXPECT_NEAR(glm::length(obj1.getAngularVel()), glm::length(obj2.getAngularVel()), 0.001f);

	// For now, verify the expected inertia relationship:
	EXPECT_FLOAT_EQ(I2 / I1, 2.0f);
}

TEST(RotationalInertia, TripleMass_RequiresTripleTorqueForSameAngularAcceleration)
{
	const float radius = 1.0f;
	const float mass1 = 1.0f;
	const float mass2 = 3.0f;
	const glm::vec3 baseTorque(0.0f, 0.0f, glm::radians(60.0f));

	PhysicsObject obj1 = CreateSphereBody(glm::vec3(0.0f), glm::vec3(0.0f), radius, mass1);
	PhysicsObject obj2 = CreateSphereBody(glm::vec3(0.0f), glm::vec3(0.0f), radius, mass2);

	obj1.SetAngularVelocity(glm::vec3(0.0f));
	obj2.SetAngularVelocity(glm::vec3(0.0f));

	float I1 = CalculateSphereInertia(mass1, radius);
	float I2 = CalculateSphereInertia(mass2, radius);
	float inertiaRatio = I2 / I1;

	// Apply torque proportional to inertia to get same angular acceleration
	obj1.addTorque(baseTorque);
	obj2.addTorque(baseTorque * inertiaRatio);

	obj1.IntegrateEuler(1.0f);
	obj2.IntegrateEuler(1.0f);

	// When properly implemented:
	 EXPECT_NEAR(glm::length(obj1.getAngularVel()), glm::length(obj2.getAngularVel()), 0.001f);

	// Verify expected inertia ratio:
	EXPECT_FLOAT_EQ(inertiaRatio, 3.0f);
}

TEST(RotationalInertia, LargerRadius_IncreasesMomentOfInertia)
{
	// For sphere: I = (2/5) * m * r²
	// Doubling radius quadruples inertia (if mass stays same)

	const float mass = 2.0f;
	const float radius1 = 1.0f;
	const float radius2 = 2.0f;
	const glm::vec3 torque(0.0f, 0.0f, glm::radians(90.0f));

	PhysicsObject obj1 = CreateSphereBody(glm::vec3(0.0f), glm::vec3(0.0f), radius1, mass);
	PhysicsObject obj2 = CreateSphereBody(glm::vec3(0.0f), glm::vec3(0.0f), radius2, mass);

	obj1.SetAngularVelocity(glm::vec3(0.0f));
	obj2.SetAngularVelocity(glm::vec3(0.0f));

	obj1.addTorque(torque);
	obj2.addTorque(torque);

	obj1.IntegrateEuler(1.0f);
	obj2.IntegrateEuler(1.0f);

	float I1 = CalculateSphereInertia(mass, radius1);
	float I2 = CalculateSphereInertia(mass, radius2);

	// I2 should be 4x I1 (radius squared relationship)
	EXPECT_FLOAT_EQ(I2 / I1, 4.0f);

	// When properly implemented:
	// obj1 should have 4x the angular velocity of obj2
	 EXPECT_NEAR(glm::length(obj1.getAngularVel()) / glm::length(obj2.getAngularVel()), 4.0f, 0.001f);
}

TEST(RotationalInertia, MultiAxisTorque_SameInertiaAllDirections_Sphere)
{
	// For a uniform sphere, moment of inertia is same about all axes through center
	const float mass = 2.0f;
	const float radius = 1.0f;
	const float torqueMagnitude = glm::radians(90.0f);

	PhysicsObject objX = CreateSphereBody(glm::vec3(0.0f), glm::vec3(0.0f), radius, mass);
	PhysicsObject objY = CreateSphereBody(glm::vec3(0.0f), glm::vec3(0.0f), radius, mass);
	PhysicsObject objZ = CreateSphereBody(glm::vec3(0.0f), glm::vec3(0.0f), radius, mass);

	objX.SetAngularVelocity(glm::vec3(0.0f));
	objY.SetAngularVelocity(glm::vec3(0.0f));
	objZ.SetAngularVelocity(glm::vec3(0.0f));

	objX.addTorque(glm::vec3(torqueMagnitude, 0.0f, 0.0f));
	objY.addTorque(glm::vec3(0.0f, torqueMagnitude, 0.0f));
	objZ.addTorque(glm::vec3(0.0f, 0.0f, torqueMagnitude));

	objX.IntegrateEuler(1.0f);
	objY.IntegrateEuler(1.0f);
	objZ.IntegrateEuler(1.0f);

	float angVelX = glm::length(objX.getAngularVel());
	float angVelY = glm::length(objY.getAngularVel());
	float angVelZ = glm::length(objZ.getAngularVel());

	// For a sphere, all three should produce same angular velocity magnitude
	// When properly implemented:
	 EXPECT_NEAR(angVelX, angVelY, 0.001f);
	 EXPECT_NEAR(angVelY, angVelZ, 0.001f);

	// Verify inertia is same for all axes:
	float I = CalculateSphereInertia(mass, radius);
	EXPECT_FLOAT_EQ(I, (2.0f / 5.0f) * mass * radius * radius);
}

TEST(RotationalInertia, ContinuousTorque_LinearAngularVelocityIncrease)
{
	// Constant torque should produce constant angular acceleration
	// ?(t) = ?? + ?t, where ? = ?/I

	const float mass = 2.0f;
	const float radius = 1.0f;
	const glm::vec3 constantTorque(0.0f, 0.0f, glm::radians(30.0f));
	const float dt = 0.1f;
	const int steps = 10;

	PhysicsObject obj = CreateSphereBody(glm::vec3(0.0f), glm::vec3(0.0f), radius, mass);
	obj.SetAngularVelocity(glm::vec3(0.0f));

	float I = CalculateSphereInertia(mass, radius);
	float expectedAlpha = glm::length(constantTorque) / I;

	std::vector<float> angularVelocities;

	for (int i = 0; i < steps; ++i)
	{
		obj.addTorque(constantTorque);
		obj.IntegrateEuler(dt);
		angularVelocities.push_back(glm::length(obj.getAngularVel()));
	}

	// When properly implemented, angular velocity should increase linearly
	// For now, just verify we're collecting data correctly:
	EXPECT_EQ(angularVelocities.size(), steps);
	EXPECT_GT(angularVelocities.back(), angularVelocities.front());

	// When inertia is implemented, uncomment to verify linear increase:
	 for (size_t i = 1; i < angularVelocities.size(); ++i)
	 {
	     float expected = expectedAlpha * dt * (i + 1);
	     EXPECT_NEAR(angularVelocities[i], expected, 0.01f);
	 }
}

TEST(RotationalInertia, ZeroMass_InfiniteInertia_NoAngularAcceleration)
{
	// Objects with zero mass (infinite inertia) should not accelerate rotationally
	const float mass = 0.0f;
	const float radius = 1.0f;
	const glm::vec3 torque(0.0f, 0.0f, glm::radians(180.0f));

	PhysicsObject obj = CreateSphereBody(glm::vec3(0.0f), glm::vec3(0.0f), radius, mass);
	obj.SetAngularVelocity(glm::vec3(0.0f));

	obj.addTorque(torque);
	obj.IntegrateEuler(1.0f);

	// With zero mass, inverse inertia should be zero, preventing rotation
	// When properly implemented:
	 EXPECT_NEAR(glm::length(obj.getAngularVel()), 0.0f, 0.001f);

	// For now verify the object reports zero mass:
	EXPECT_FLOAT_EQ(obj.getMass(), 0.0f);
}

TEST(RotationalInertia, TorqueRatioMatchesMassRatio_TwoObjects)
{
	// If torque ratio matches mass ratio, angular accelerations should be equal
	const float radius = 1.0f;
	const float mass1 = 1.5f;
	const float mass2 = 4.5f;
	const float massRatio = mass2 / mass1; // 3.0

	const glm::vec3 torque1(0.0f, 0.0f, glm::radians(50.0f));
	const glm::vec3 torque2 = torque1 * massRatio;

	PhysicsObject obj1 = CreateSphereBody(glm::vec3(0.0f), glm::vec3(0.0f), radius, mass1);
	PhysicsObject obj2 = CreateSphereBody(glm::vec3(0.0f), glm::vec3(0.0f), radius, mass2);

	obj1.SetAngularVelocity(glm::vec3(0.0f));
	obj2.SetAngularVelocity(glm::vec3(0.0f));

	obj1.addTorque(torque1);
	obj2.addTorque(torque2);

	obj1.IntegrateEuler(1.0f);
	obj2.IntegrateEuler(1.0f);

	float I1 = CalculateSphereInertia(mass1, radius);
	float I2 = CalculateSphereInertia(mass2, radius);
	float inertiaRatio = I2 / I1;

	// Verify mass ratio equals inertia ratio (for same radius)
	EXPECT_FLOAT_EQ(massRatio, inertiaRatio);

	// When properly implemented, angular velocities should be equal:
	 EXPECT_NEAR(glm::length(obj1.getAngularVel()), glm::length(obj2.getAngularVel()), 0.001f);
}

TEST(RotationalInertia, SemiImplicitEuler_SameInertiaRelationship)
{
	// Test that inertia relationship holds for semi-implicit Euler integrator too
	const float radius = 1.0f;
	const float mass1 = 2.0f;
	const float mass2 = 6.0f;
	const glm::vec3 torque(0.0f, 0.0f, glm::radians(120.0f));

	PhysicsObject obj1 = CreateSphereBody(glm::vec3(0.0f), glm::vec3(0.0f), radius, mass1);
	PhysicsObject obj2 = CreateSphereBody(glm::vec3(0.0f), glm::vec3(0.0f), radius, mass2);

	obj1.SetAngularVelocity(glm::vec3(0.0f));
	obj2.SetAngularVelocity(glm::vec3(0.0f));

	obj1.addTorque(torque);
	obj2.addTorque(torque);

	obj1.IntegrateSemiImplicitEuler(1.0f);
	obj2.IntegrateSemiImplicitEuler(1.0f);

	float I1 = CalculateSphereInertia(mass1, radius);
	float I2 = CalculateSphereInertia(mass2, radius);
	float expectedRatio = I2 / I1; // Should be 3.0

	// When properly implemented:
	 EXPECT_NEAR(glm::length(obj1.getAngularVel()) / glm::length(obj2.getAngularVel()), expectedRatio, 0.001f);

	EXPECT_FLOAT_EQ(expectedRatio, 3.0f);
}










