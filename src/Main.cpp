#include <raylib.h>

void main()
{
	InitWindow(1280, 720, "Ejemplo");
	while (!WindowShouldClose())
	{	
		BeginDrawing();
		ClearBackground(BLACK);
		EndDrawing();
	}
	CloseWindow();
}