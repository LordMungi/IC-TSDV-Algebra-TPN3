#pragma once
#include <raylib.h>

class AABB
{
private:

	Vector3 position;

	Vector3 min;
	Vector3 max;

	void calculateSize(Mesh mesh);
public:

	AABB();

	void setAABB(Mesh mesh, Vector3 meshPosition, Vector3 meshRotation);
	void updateAABB(Matrix transform);

	inline void setPosition(Vector3 position) { this->position = position; };

	bool isColliding(AABB other);

	void render();
};