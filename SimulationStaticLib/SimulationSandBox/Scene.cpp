#include "Scene.h"

bool Scene::loadFromBinaryFile(const std::string& path)
{
	FlatBufferLoader loader;
	return loader.loadSceneFromFile(path, *this);
}
