#pragma once
#include <raylib.h>

class AABB
{
private:

	Vector3 position;
	Vector3 size;
	Vector3 rotation;

	Vector3 calculateSize(Mesh mesh);
public:

	AABB();

	void setAABB(Mesh mesh, Vector3 meshPosition, Vector3 meshRotation);
	inline void setPosition(Vector3 position) { this->position = position; };

	bool isColliding(AABB other);

	void render();
};