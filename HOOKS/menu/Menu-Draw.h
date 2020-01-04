#pragma once
#include "../../includes.h"
#include "../../SDK/ISurface.h"
#include <array>


namespace MENU_DRAW
{
	void FilledRectManual(int x, int y, int w, int h, CColor color);
	void OutlinedRect(int x, int y, int w, int h, CColor color);
	void OutlinedVecRect(int x, int y, int x2, int y2, CColor color);
	void DrawTexturedPoly(int n, SDK::Vertex_t* vertice, CColor col);
	void Line(int x, int y, int x2, int y2, CColor color);
	void DrawFilledCircle(Vector2D center, CColor color, float radius, float points);
	void DrawT(int X, int Y, CColor Color, int Font, bool Center, const char* _Input, ...);
	RECT GetTextSizeRect(DWORD font, const char* text);
	Vector2D GetMousePosition();
	bool MouseInRegion(int x, int y, int x2, int y2);
}
