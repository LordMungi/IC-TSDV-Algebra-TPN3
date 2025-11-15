#include "aabb.h"
#include <raymath.h>


AABB::AABB()
{
	position = { 0,0,0 };
	min = { 0,0,0 };
	max = { 0,0,0 };
}

void AABB::calculateSize(Mesh mesh)
{
	min = { _FMAX,  _FMAX,  _FMAX };
	max = { -_FMAX, -_FMAX, -_FMAX };
	
	for (int i = 0; i < mesh.vertexCount * 3; i += 3)
	{
		min = Vector3Min(min, { mesh.vertices[i], mesh.vertices[i + 1], mesh.vertices[i + 2] });
		max = Vector3Max(max, { mesh.vertices[i], mesh.vertices[i + 1], mesh.vertices[i + 2] });
	}
}

void AABB::updateAABB(Matrix transform)
{
	Vector3 corners[8] =
	{
		{min.x, min.y, min.z},
		{max.x, min.y, min.z},
		{min.x, max.y, min.z},
		{min.x, min.y, max.z},
		{max.x, max.y, max.z},
		{min.x, max.y, max.z},
		{max.x, min.y, max.z},
		{max.x, max.y, min.z},
	};

	for (int i = 0; i < 8; i++)
		Vector3Transform(corners[i], transform);

	min = corners[0];
	max = corners[0];

	for (int i = 0; i < 8; i++)
	{
		min = Vector3Min(min, corners[i]);
		max = Vector3Max(max, corners[i]);
	}
}

void AABB::setAABB(Mesh mesh, Vector3 meshPosition, Vector3 meshRotation)
{
	position = meshPosition;
	calculateSize(mesh);
}

bool AABB::isColliding(AABB other)
{
	return false;
}

void AABB::render()
{
	DrawCubeWires(position, max.x, max.y, max.z, GRAY);
}