#include "aabb.h"
#include <raymath.h>


AABB::AABB()
{
	position = { 0,0,0 };
	size = { 0,0,0 };
	rotation = { 0,0,0 };
}

Vector3 AABB::calculateSize(Mesh mesh)
{
	Vector3 min = { _FMAX,  _FMAX,  _FMAX };
	Vector3 max = { -_FMAX, -_FMAX, -_FMAX };

	for (int i = 0; i < mesh.vertexCount * 3; i += 3)
	{
		min = Vector3Min(min, { mesh.vertices[i], mesh.vertices[i + 1], mesh.vertices[i + 2] });
		max = Vector3Max(max, { mesh.vertices[i], mesh.vertices[i + 1], mesh.vertices[i + 2] });
	}

	return Vector3Subtract(max, min);
}

void AABB::setAABB(Mesh mesh, Vector3 meshPosition, Vector3 meshRotation)
{
	position = meshPosition;

	size = calculateSize(mesh);

	rotation = meshRotation;
}

bool AABB::isColliding(AABB other)
{
	return false;
}

void AABB::render()
{
	DrawCubeWires(position, size.x, size.y, size.z, GRAY);
}