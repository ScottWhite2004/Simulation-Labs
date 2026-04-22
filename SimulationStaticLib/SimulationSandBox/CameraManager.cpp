#include "CameraManager.h"
void CameraManager::addCamera(const std::string& name, const Camera& camera)
{
	const bool wasEmpty = _cameras.empty();

	_cameras[name] = camera;

	// First camera added becomes active by default.
	if (wasEmpty) {
		_currentCameraName = name;
		_currentCamera = camera;
	}
	// If replacing the currently active camera, keep current in sync.
	else if (_currentCameraName == name) {
		_currentCamera = camera;
	}
}

bool CameraManager::switchToCamera(const std::string& name)
{
	const auto it = _cameras.find(name);
	if (it == _cameras.end()) {
		return false;
	}

	_currentCameraName = name;
	_currentCamera = it->second;
	_localCamera = it->second; // Update local camera as well.
	return true;
}

void CameraManager::rotateCurrentCamera(float yaw, float pitch)
{
	_localCamera.rotateCamera(yaw, pitch);

	// Keep dictionary copy in sync with the active camera.
	if (!_currentCameraName.empty()) {
		_cameras[_currentCameraName] = _currentCamera;
	}
}

void CameraManager::panCurrentCamera(float rightUnits, float forwardUnits, float upUnits)
{
	_localCamera.panCamera(rightUnits, forwardUnits, upUnits);

	// Keep dictionary copy in sync with the active camera.
	if (!_currentCameraName.empty()) {
		_cameras[_currentCameraName] = _currentCamera;
	}
}

void CameraManager::resetLocalCamera()
{
	if (!_currentCameraName.empty()) {
		_localCamera = _cameras[_currentCameraName];
	}
}