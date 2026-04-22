#define GLM_ENABLE_EXPERIMENTAL
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>


glm::mat4 Camera::getProjectionMatrix() const
{
    if (_projectionType == ProjectionType::Orthographic)
    {
		const float halfHeight = _orthoHeight * 0.5f;
        const float halfWidth = halfHeight * _aspect;
		return glm::orthoZO(-halfWidth, halfWidth, -halfHeight, halfHeight, _near, _far);
    }
    return glm::perspectiveZO(glm::radians(_fovy), _aspect, _near, _far);
}

void Camera::rotateCamera(float yawRadians, float pitchRadians)
{
    const glm::vec3 eye = _eye;
    const glm::vec3 center = _center;
    glm::vec3 up = _up;

    glm::vec3 dir = center - eye;
    if (glm::length(dir) < 1e-6f) {
        return;
    }
    dir = glm::normalize(dir);

    // Yaw around the current up axis
    if (std::abs(yawRadians) > 0.0f) {
        dir = glm::normalize(glm::rotate(dir, yawRadians, up));
    }

    // Pitch around the right axis (perpendicular to dir and up)
    glm::vec3 right = glm::normalize(glm::cross(dir, up));
    if (std::abs(pitchRadians) > 0.0f) {
        dir = glm::normalize(glm::rotate(dir, pitchRadians, right));
        // Recompute up to keep it orthogonal
        up = glm::normalize(glm::cross(right, dir));
    }

    setCenter(eye + dir);
    setUp(up);
}

void Camera::panCamera(float rightUnits, float forwardUnits, float upUnits)
{
    const glm::vec3 forward = glm::normalize(_center - _eye);
    const glm::vec3 right = glm::normalize(glm::cross(forward, _up));
    const glm::vec3 up = glm::normalize(glm::cross(right, forward));
    _eye += right * rightUnits + forward * forwardUnits + up * upUnits;
    _center += right * rightUnits + forward * forwardUnits + up * upUnits;
}
