#include "program.h"

#include "figure.h"

namespace program
{
	static void init();
	static void update();
	static void draw();
	static void end();

	const int gridPrecision = 5;

	Camera3D camera;
	Vector3 cubePosition = { 0, 0, 0 };

	const int maxFigures = 6;

	Figure figures[maxFigures];
	Figure* selectedFigure;

	Vector3 intersectionMin;
	Vector3 intersectionMax;

	bool cursorMode;

	bool aabbCollision;
	bool meshCollision;

	void run()
	{
		init();

		while (!WindowShouldClose())
		{
			update();
			draw();
		}
		
		end();
	}

	static void init()
	{
		InitWindow(1024, 768, "Ejemplo");

		camera.position = { 0, 10, 10 };
		camera.target = { 0, 0, 0 };
		camera.up = { 0, 1, 0 };
		camera.fovy = 45.0f;
		camera.projection = CAMERA_PERSPECTIVE;
		
		figures[0] = Figure(LoadModel("resource/cube.obj"), { 0, 0, 0});
		figures[1] = Figure(LoadModel("resource/decahedron.obj"), { 2, -1, 2 });
		figures[2] = Figure(LoadModel("resource/dodecahedron.obj"), { -2, 0, 2 });
		figures[3] = Figure(LoadModel("resource/icosahedron.obj"), { 2, 0, -2 });
		figures[4] = Figure(LoadModel("resource/octahedron.obj"), { -2, 0, -2 });
		figures[5] = Figure(LoadModel("resource/tetrahedron.obj"), { 0, 2, 0 });

		selectedFigure = &figures[0];

		DisableCursor();
		cursorMode = false;
	}

	static void update()
	{
		float delta = GetFrameTime();

		aabbCollision = false;
		meshCollision = false;

		if (!cursorMode)
			UpdateCamera(&camera, CAMERA_FREE);
		else
		{
			if (IsKeyDown(KEY_W))
			{
				if (IsKeyDown(KEY_LEFT_SHIFT))
					selectedFigure->rotate({ -1,0,0 }, delta);
				else
					selectedFigure->move({ 0,0,-1 }, delta);
			}
			if (IsKeyDown(KEY_A))
			{
				if (IsKeyDown(KEY_LEFT_SHIFT))
					selectedFigure->rotate({ 0,0,1 }, delta);
				else
					selectedFigure->move({ -1,0,0 }, delta);
			}
			if (IsKeyDown(KEY_S))
			{
				if (IsKeyDown(KEY_LEFT_SHIFT))
					selectedFigure->rotate({ 1,0,0 }, delta);
				else
					selectedFigure->move({ 0,0,1 }, delta);
			}
			if (IsKeyDown(KEY_D))
			{
				if (IsKeyDown(KEY_LEFT_SHIFT))
					selectedFigure->rotate({ 0,0,-1 }, delta);
				else
					selectedFigure->move({ 1,0,0 }, delta);
			}
			if (IsKeyDown(KEY_E))
			{
				if (IsKeyDown(KEY_LEFT_SHIFT))
					selectedFigure->rotate({ 0,1,0 }, delta);
				else
					selectedFigure->move({ 0,1,0 }, delta);
			}
			if (IsKeyDown(KEY_Q))
			{
				if (IsKeyDown(KEY_LEFT_SHIFT))
					selectedFigure->rotate({ 0,-1,0 }, delta);
				else
					selectedFigure->move({ 0,-1,0}, delta);
			}

			if (IsKeyDown(KEY_ONE))
			{
				if (IsKeyDown(KEY_LEFT_SHIFT))
					selectedFigure->scale({ -1,0,0 }, delta);
				else
					selectedFigure->scale({ 1,0,0 }, delta);
			}
			if (IsKeyDown(KEY_TWO))
			{
				if (IsKeyDown(KEY_LEFT_SHIFT))
					selectedFigure->scale({ 0,-1,0 }, delta);
				else
					selectedFigure->scale({ 0,1,0 }, delta);
			}
			if (IsKeyDown(KEY_THREE))
			{
				if (IsKeyDown(KEY_LEFT_SHIFT))
					selectedFigure->scale({ 0,0,-1 }, delta);
				else
					selectedFigure->scale({ 0,0,1 }, delta);
			}
		}

		if (IsKeyPressed(KEY_TAB))
		{
			if (cursorMode)
			{
				DisableCursor();
				cursorMode = false;
			}
			else
			{
				EnableCursor();
				cursorMode = true;
			}
		}

		intersectionMin = { 0,0,0 };
		intersectionMax = { 0,0,0 };

		for (int i = 0; i < maxFigures; i++)
		{
			for (int j = i + 1; j < maxFigures; j++)
			{
				if (figures[i].isColliding(figures[j]))
				{
					aabbCollision = true;

					intersectionMin = Vector3Max(figures[i].getAABBMin(), figures[j].getAABBMin());
					intersectionMax = Vector3Min(figures[i].getAABBMax(), figures[j].getAABBMax());
					Vector3 size = Vector3Subtract(intersectionMax, intersectionMin);

					Vector3 step;
					step.x = size.x / (gridPrecision - 1);
					step.y = size.y / (gridPrecision - 1);
					step.z = size.z / (gridPrecision - 1);

					Vector3 point = { 0,0,0 };

					bool shouldStop = false;

					for (int x = 0; x < gridPrecision && !shouldStop; x++)
					{
						point.x = intersectionMin.x + step.x * x;

						for (int y = 0; y < gridPrecision && !shouldStop; y++)
						{
							point.y = intersectionMin.y + step.y * y;

							for (int z = 0; z < gridPrecision && !shouldStop; z++)
							{
								point.z = intersectionMin.z + step.z * z;

								if (figures[i].isPointInside(point) && figures[j].isPointInside(point))
								{
									shouldStop = true;
									meshCollision = true;
								}
							}
						}
					}
				}
			}
		}

	}

	static void draw()
	{
		BeginDrawing();

		ClearBackground(RAYWHITE);
		
		BeginMode3D(camera);

		for (int i = 0; i < maxFigures; i++)
			figures[i].render();

		DrawGrid(10, 1);
		DrawCubeWires(Vector3Scale(Vector3Add(intersectionMin, intersectionMax), 0.5f), intersectionMax.x - intersectionMin.x, intersectionMax.y - intersectionMin.y, intersectionMax.z - intersectionMin.z, GREEN);

		EndMode3D();

		if (aabbCollision)
		{
			if (meshCollision)
				DrawText("Mesh Collision", 20, 50, 20, RED);
			else
				DrawText("AABB Collision", 20, 50, 20, ORANGE);
		}
		
		EndDrawing();
	}

	static void end()
	{
		CloseWindow();
	}
}