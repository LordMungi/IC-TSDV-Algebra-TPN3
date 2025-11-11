#include <raylib.h>

void main()
{
	InitWindow(1024, 768, "Ejemplo");
	while (!WindowShouldClose())
	{	
		BeginDrawing();
		ClearBackground(BLACK);
		EndDrawing();
	}
	CloseWindow();
}