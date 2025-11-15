#include "aabb.h"

AABB::AABB()
{
	localMin = { 0,0,0 };
	localMax = { 0,0,0 };
	min = { 0,0,0 };
	max = { 0,0,0 };
}

void AABB::calculateSize(Mesh mesh)
{
	localMin = { _FMAX,  _FMAX,  _FMAX };
	localMax = { -_FMAX, -_FMAX, -_FMAX };
	
	for (int i = 0; i < mesh.vertexCount * 3; i += 3)
	{
		localMin = Vector3Min(localMin, { mesh.vertices[i], mesh.vertices[i + 1], mesh.vertices[i + 2] });
		localMax = Vector3Max(localMax, { mesh.vertices[i], mesh.vertices[i + 1], mesh.vertices[i + 2] });
	}
}

void AABB::update(Matrix transform)
{
	Vector3 corners[8] =
	{
		{localMin.x, localMin.y, localMin.z},
		{localMax.x, localMin.y, localMin.z},
		{localMin.x, localMax.y, localMin.z},
		{localMin.x, localMin.y, localMax.z},
		{localMax.x, localMax.y, localMax.z},
		{localMin.x, localMax.y, localMax.z},
		{localMax.x, localMin.y, localMax.z},
		{localMax.x, localMax.y, localMin.z},
	};

	for (int i = 0; i < 8; i++)
		corners[i] = Vector3Transform(corners[i], transform);

	min = corners[0];
	max = corners[0];

	for (int i = 0; i < 8; i++)
	{
		min = Vector3Min(min, corners[i]);
		max = Vector3Max(max, corners[i]);
	}
}

void AABB::setAABB(Mesh mesh)
{
	calculateSize(mesh);
}

bool AABB::isColliding(AABB other)
{
	if (max.x < other.min.x || min.x > other.max.x) return false;
	if (max.y < other.min.y || min.y > other.max.y) return false;
	if (max.z < other.min.z || min.z > other.max.z) return false;
	return true;
}

void AABB::render()
{
	DrawCubeWires(Vector3Scale(Vector3Add(min, max), 0.5f), max.x - min.x, max.y - min.y, max.z - min.z, GRAY);
}