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

	const int maxFigures = 6;

	Figure figures[maxFigures];
	Figure* selectedFigure;

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

	}

	static void draw()
	{
		BeginDrawing();

		ClearBackground(RAYWHITE);
		
		BeginMode3D(camera);

		for (int i = 0; i < maxFigures; i++)
			figures[i].render();

		DrawGrid(10, 1);

		EndMode3D();

		if (aabbCollision)
			DrawText("AABB Collision", 20, 50, 20, ORANGE);
		
		EndDrawing();
	}

	static void end()
	{
		CloseWindow();
	}
}