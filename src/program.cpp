#include "program.h"
#include <raylib.h>

namespace program
{
	static void init();
	static void update();
	static void draw();
	static void end();

	Camera3D camera;
	Vector3 cubePosition = { 0, 0, 0 };

	bool cursorMode;

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

		DisableCursor();
		cursorMode = false;
	}

	static void update()
	{
		if (!cursorMode)
			UpdateCamera(&camera, CAMERA_FREE);

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
	}

	static void draw()
	{
		BeginDrawing();

		ClearBackground(RAYWHITE);
		
		BeginMode3D(camera);

		DrawCube(cubePosition, 2, 2, 2, RED);
		DrawCubeWires(cubePosition, 2, 2, 2, MAROON);
		DrawGrid(10, 1);

		EndMode3D();
		
		EndDrawing();
	}

	static void end()
	{
		CloseWindow();
	}
}