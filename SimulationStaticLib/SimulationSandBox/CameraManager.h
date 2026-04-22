#pragma once
#include "Camera.h"
#include <string>
#include <unordered_map>

class CameraManager
{
	std::unordered_map<std::string, Camera> _cameras;
	Camera _currentCamera;
	std::string _currentCameraName;
	Camera _localCamera;

public:
	CameraManager() = default;
	~CameraManager() = default;

	void addCamera(const std::string& name, const Camera& camera);
	bool switchToCamera(const std::string& name);

	const Camera getCurrentCamera() const { return _currentCamera; }
	const std::string& getCurrentCameraName() const { return _currentCameraName; }
	const Camera& getLocalCamera() const { return _localCamera; }

	void rotateCurrentCamera(float yaw, float pitch);
	void panCurrentCamera(float rightUnits, float forwardUnits, float upUnits);
	void setCurrentCameraAspect(float aspect) { _currentCamera.setAspect(aspect); }
	void resetLocalCamera();
	std::unordered_map<std::string, Camera> getCameras() const { return _cameras; }
};

