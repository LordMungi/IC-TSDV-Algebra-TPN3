#include "program.h"

#include "figure.h"


namespace program
{
	static void init();
	static void update();
	static void draw();
	static void end();

	Camera3D camera;
	Vector3 cubePosition = { 0, 0, 0 };

	Figure cube;
	Figure dodecahedron;

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

		cube = Figure(LoadModel("resource/cube.obj"));
		dodecahedron = Figure(LoadModel("resource/dodecahedron.obj"));

		DisableCursor();
		cursorMode = false;
	}

	static void update()
	{
		float delta = GetFrameTime();

		if (!cursorMode)
			UpdateCamera(&camera, CAMERA_FREE);
		else
		{
			if (IsKeyDown(KEY_W))
			{
				if (IsKeyDown(KEY_LEFT_SHIFT))
					cube.rotate({ -1,0,0 }, delta);
				else
					cube.move({ 0,0,-1 }, delta);
			}
			if (IsKeyDown(KEY_A))
			{
				if (IsKeyDown(KEY_LEFT_SHIFT))
					cube.rotate({ 0,0,1 }, delta);
				else
					cube.move({ -1,0,0 }, delta);
			}
			if (IsKeyDown(KEY_S))
			{
				if (IsKeyDown(KEY_LEFT_SHIFT))
					cube.rotate({ 1,0,0 }, delta);
				else
					cube.move({ 0,0,1 }, delta);
			}
			if (IsKeyDown(KEY_D))
			{
				if (IsKeyDown(KEY_LEFT_SHIFT))
					cube.rotate({ 0,0,-1 }, delta);
				else
					cube.move({ 1,0,0 }, delta);
			}
			if (IsKeyDown(KEY_E))
			{
				if (IsKeyDown(KEY_LEFT_SHIFT))
					cube.rotate({ 0,1,0 }, delta);
				else
					cube.move({ 0,1,0 }, delta);
			}
			if (IsKeyDown(KEY_Q))
			{
				if (IsKeyDown(KEY_LEFT_SHIFT))
					cube.rotate({ 0,-1,0 }, delta);
				else
					cube.move({ 0,-1,0}, delta);
			}

			if (IsKeyDown(KEY_ONE))
			{
				if (IsKeyDown(KEY_LEFT_SHIFT))
					cube.scale({ -1,0,0 }, delta);
				else
					cube.scale({ 1,0,0 }, delta);
			}
			if (IsKeyDown(KEY_TWO))
			{
				if (IsKeyDown(KEY_LEFT_SHIFT))
					cube.scale({ 0,-1,0 }, delta);
				else
					cube.scale({ 0,1,0 }, delta);
			}
			if (IsKeyDown(KEY_THREE))
			{
				if (IsKeyDown(KEY_LEFT_SHIFT))
					cube.scale({ 0,0,-1 }, delta);
				else
					cube.scale({ 0,0,1 }, delta);
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


	}

	static void draw()
	{
		BeginDrawing();

		ClearBackground(RAYWHITE);
		
		BeginMode3D(camera);

		cube.render();
		dodecahedron.render();
		DrawGrid(10, 1);

		EndMode3D();

		if (cube.isColliding(dodecahedron))
			DrawText("AABB Collision", 20, 50, 20, ORANGE);
		
		EndDrawing();
	}

	static void end()
	{
		CloseWindow();
	}
}