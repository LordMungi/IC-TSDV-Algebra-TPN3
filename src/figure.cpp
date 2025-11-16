#include "figure.h"

Figure::Figure()
{
	position = { 0,0,0 };
	rotation = { 0,0,0 };
	size = { 1,1,1 };
	color = RED;

	isSelected = false;
}

Figure::Figure(Model model)
{
	this->model = model;

	position = { 0,0,0 };
	rotation = { 0,0,0 };
	size = { 1,1,1 };
	color = RED;

	getLocalCenter();
	aabb.setAABB(model.meshes[0]);
	aabb.update(model.transform);

	isSelected = false;
}

Figure::Figure(Model model, Vector3 position, Color color)
{
	this->model = model;

	this->position = position;
	rotation = { 0,0,0 };
	size = { 1,1,1 };
	this->color = color;

	getLocalCenter();
	aabb.setAABB(model.meshes[0]);
	aabb.update(model.transform);

	isSelected = false;
	applyTransform();
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

bool Figure::isPointInside(Vector3 point)
{
	Mesh* mesh = &model.meshes[0];
	for (int i = 0; i < mesh->triangleCount; i++)
	{
		Vector3 v1 = { 0,0,0 };
		Vector3 v2 = { 0,0,0 };
		Vector3 v3 = { 0,0,0 };

		if (mesh->indices)
		{
			v1 = { mesh->vertices[mesh->indices[i * 3 + 0] * 3 + 0], mesh->vertices[mesh->indices[i * 3 + 0] * 3 + 1] , mesh->vertices[mesh->indices[i * 3 + 0] * 3 + 2] };
			v2 = { mesh->vertices[mesh->indices[i * 3 + 1] * 3 + 0], mesh->vertices[mesh->indices[i * 3 + 1] * 3 + 1] , mesh->vertices[mesh->indices[i * 3 + 1] * 3 + 2] };
			v3 = { mesh->vertices[mesh->indices[i * 3 + 2] * 3 + 0], mesh->vertices[mesh->indices[i * 3 + 2] * 3 + 1] , mesh->vertices[mesh->indices[i * 3 + 2] * 3 + 2] };
		}
		else
		{
			v1 = { mesh->vertices[(i * 3 + 0) * 3 + 0], mesh->vertices[(i * 3 + 0) * 3 + 1], mesh->vertices[(i * 3 + 0) * 3 + 2] };
			v2 = { mesh->vertices[(i * 3 + 1) * 3 + 0], mesh->vertices[(i * 3 + 1) * 3 + 1], mesh->vertices[(i * 3 + 1) * 3 + 2] };
			v3 = { mesh->vertices[(i * 3 + 2) * 3 + 0], mesh->vertices[(i * 3 + 2) * 3 + 1], mesh->vertices[(i * 3 + 2) * 3 + 2] };
		}

		v1 = Vector3Transform(v1, model.transform);
		v2 = Vector3Transform(v2, model.transform);
		v3 = Vector3Transform(v3, model.transform);

		Vector3 edge1 = Vector3Subtract(v2, v1);
		Vector3 edge2 = Vector3Subtract(v3, v1);
		Vector3 normal = Vector3Normalize(Vector3CrossProduct(edge1, edge2));

		Vector3 worldCenter = Vector3Transform(center, model.transform);

		if (Vector3DotProduct(normal, Vector3Subtract(worldCenter, v1)) > 0)
			normal = Vector3Negate(normal);

		if (Vector3DotProduct(Vector3Subtract(point, v1), normal) > 0.0001f)
			return false;
	}	
	return true;
}


void Figure::render()
{
	if (isSelected)
		DrawModel(model, { 0,0,0 }, 1.0f, RED);
	else
		DrawModel(model, { 0,0,0 }, 1.0f, LIGHTGRAY);
	aabb.render();
}