#pragma once
#include "aabb.h"

class Figure
{
private:

	Model model;
	AABB aabb;

	Vector3 position;
	Vector3 rotation;
	Vector3 size;

	Vector3 center;

	void applyTransform();
	void getLocalCenter();

public:

	Figure();
	Figure(Model model);
	Figure(Model model, Vector3 position, Vector3 size);

	void move(Vector3 direction, float delta);
	void rotate(Vector3 angle, float delta);
	void scale(Vector3 axis, float delta);

	inline bool isColliding(Figure other) { return aabb.isColliding(other.aabb); };

	void render();
};