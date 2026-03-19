#pragma once
#include "Camera.h"
#include <vector>

class CameraManager
{
	std::vector<Camera> _cameras;
	Camera _currentCamera;

public:
	CameraManager() = default;
	~CameraManager() = default;
	void addCamera(const Camera& camera);
	bool switchToCamera(const int& index);
	const Camera getCurrentCamera() const { return _currentCamera; }
	void rotateCurrentCamera(float yaw, float pitch);
	void panCurrentCamera(float rightUnits, float forwardUnits, float upUnits);
	void setCurrentCameraAspect(float aspect) { _currentCamera.setAspect(aspect); }
};

