#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum ProjectionType
{
	Perspective,
	Orthographic
};

class Camera
{
	glm::vec3 _eye;
	glm::vec3 _center;
	glm::vec3 _up;

	float _fovy;
	float _aspect;
	float _near;
	float _far;
	float _orthoHeight = 10.0f;

	ProjectionType _projectionType = ProjectionType::Perspective;

public:
	Camera() = default;
	Camera(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up, float fovy, float aspect, float near, float far, float orthoHeight, ProjectionType projectionType) : _eye(eye), _center(center), _up(up), _fovy(fovy), _aspect(aspect), _near(near), _far(far), _orthoHeight(orthoHeight), _projectionType(projectionType) {}
	~Camera() = default;

	inline glm::mat4 getViewMatrix() const
	{
		return glm::lookAt(_eye, _center, _up);
	}
	glm::mat4 getProjectionMatrix() const;

	void setEye(const glm::vec3& eye) { _eye = eye; }
	void setCenter(const glm::vec3& center) { _center = center; }
	void setUp(const glm::vec3& up) { _up = up; }
	void setFovy(const float& fovy) { _fovy = fovy; }
	void setAspect(const float& aspect) { _aspect = aspect; }
	void setNear(const float& near) { _near = near; }
	void setFar(const float& far) { _far = far; }
	void setOrthoHeight(const float& orthoHeight) { _orthoHeight = orthoHeight; }
	void setProjectionType(const ProjectionType& projectionType) { _projectionType = projectionType; }

	glm::vec3 getEye() const { return _eye; }
	glm::vec3 getCenter() const { return _center; }
	glm::vec3 getUp() const { return _up; }
	float getFovy() const { return _fovy; }
	float getAspect() const { return _aspect; }
	float getNear() const { return _near; }
	float getFar() const { return _far; }

	void rotateCamera(float yaw, float pitch);
	void panCamera(float rightUnits, float forwardUnits, float upUnits);
};

