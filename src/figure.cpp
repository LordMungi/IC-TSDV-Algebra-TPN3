#include "figure.h"

#include <raymath.h>

Figure::Figure()
{
	position = { 0,0,0 };
	rotation = { 0,0,0 };
	size = { 1,1,1 };
}

Figure::Figure(Model model)
{
	this->model = model;

	position = { 0,0,0 };
	rotation = { 0,0,0 };
	size = { 1,1,1 };
	getLocalCenter();
	aabb.setAABB(model.meshes[0], position, rotation);
	aabb.update(model.transform);
}

Figure::Figure(Model model, Vector3 position, Vector3 size)
{
	this->model = model;
	this->position = position;
	this->size = size;
	rotation = { 0,0,0 };
	getLocalCenter();
	aabb.setAABB(model.meshes[0], position, rotation);
	aabb.update(model.transform);
}

void Figure::getLocalCenter()
{
	Vector3 min = { _FMAX,  _FMAX,  _FMAX };
	Vector3 max = { -_FMAX, -_FMAX, -_FMAX };

	for (int i = 0; i < model.meshes[0].vertexCount * 3; i += 3)
	{
		min = Vector3Min(min, { model.meshes[0].vertices[i], model.meshes[0].vertices[i + 1], model.meshes[0].vertices[i + 2] });
		max = Vector3Max(max, { model.meshes[0].vertices[i], model.meshes[0].vertices[i + 1], model.meshes[0].vertices[i + 2] });
	}

	center = Vector3Scale(Vector3Add(max, min), 0.5f);
}

void Figure::applyTransform()
{
	Matrix translate = MatrixTranslate(position.x, position.y, position.z);
	Matrix rotX = MatrixRotateX(rotation.x * DEG2RAD);
	Matrix rotY = MatrixRotateY(rotation.y * DEG2RAD);
	Matrix rotZ = MatrixRotateZ(rotation.z * DEG2RAD);
	Matrix scale = MatrixScale(size.x, size.y, size.z);

	Matrix rotate(MatrixMultiply(MatrixMultiply(rotZ, rotY), rotX));

	Matrix pivot = MatrixTranslate(center.x, center.y, center.z);
	Matrix negPivot = MatrixTranslate(-center.x, -center.y, -center.z);

	model.transform = MatrixMultiply(MatrixMultiply(MatrixMultiply(pivot, MatrixMultiply(rotate, scale)), negPivot), translate);
	aabb.update(model.transform);
}

void Figure::move(Vector3 direction, float delta)
{
	float speed = 2;
	
	position.x += direction.x * delta * speed;
	position.y += direction.y * delta * speed;
	position.z += direction.z * delta * speed;

	applyTransform();
}

void Figure::rotate(Vector3 angle, float delta)
{
	float speed = 40;

	rotation.x += angle.x * delta * speed;
	rotation.y += angle.y * delta * speed;
	rotation.z += angle.z * delta * speed;

	applyTransform();
}

void Figure::scale(Vector3 axis, float delta)
{
	float speed = 2;

	size.x += axis.x * delta * speed;
	size.y += axis.y * delta * speed;
	size.z += axis.z * delta * speed;

	applyTransform();
}


void Figure::render()
{
	DrawModel(model, { 0,0,0 }, 1.0f, RED);
	aabb.render();
}