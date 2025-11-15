#pragma once
#include <raylib.h>

class AABB
{
private:

	Vector3 localMin;
	Vector3 localMax;

	Vector3 min;
	Vector3 max;

	void calculateSize(Mesh mesh);
public:

	AABB();

	void setAABB(Mesh mesh);
	void update(Matrix transform);

	bool isColliding(AABB other);

	void render();
};