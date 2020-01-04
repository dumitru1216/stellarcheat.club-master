#include "../../includes.h"
#include "../../SDK/ISurface.h"
#include <array>
#include "Menu-Draw.h"

namespace MENU_DRAW
{
	void FilledRectManual(int x, int y, int w, int h, CColor color)
	{
		INTERFACES::Surface->DrawSetColor(color.RGBA[0], color.RGBA[1], color.RGBA[2], color.RGBA[3]);
		INTERFACES::Surface->DrawFilledRect(x, y, w, h);
	}

	void OutlinedRect(int x, int y, int w, int h, CColor color)
	{
		INTERFACES::Surface->DrawSetColor(color.RGBA[0], color.RGBA[1], color.RGBA[2], color.RGBA[3]);
		INTERFACES::Surface->DrawOutlinedRect(x, y, x + w, y + h);
	}

	void OutlinedVecRect(int x, int y, int x2, int y2, CColor color)
	{
		INTERFACES::Surface->DrawSetColor(color.RGBA[0], color.RGBA[1], color.RGBA[2], color.RGBA[3]);
		INTERFACES::Surface->DrawLine(x, y, x2, y);
		INTERFACES::Surface->DrawLine(x2, y, x2, y2);
		INTERFACES::Surface->DrawLine(x2, y2, x, y2);
		INTERFACES::Surface->DrawLine(x, y2, x, y);
	}

	void Line(int x, int y, int x2, int y2, CColor color)
	{
		INTERFACES::Surface->DrawSetColor(color.RGBA[0], color.RGBA[1], color.RGBA[2], color.RGBA[3]);
		INTERFACES::Surface->DrawLine(x, y, x2, y2);
	}

	void DrawTexturedPoly(int n, SDK::Vertex_t* vertice, CColor col) {
		static int texture_id = INTERFACES::Surface->CreateNewTextureID(true);
		static unsigned char buf[4] = { 255, 255, 255, 255 };
		INTERFACES::Surface->DrawSetTextureRGBA(texture_id, buf, 1, 1);
		INTERFACES::Surface->DrawSetColor(col);
		INTERFACES::Surface->DrawSetTexture(texture_id);
		INTERFACES::Surface->DrawTexturedPolygon(n, vertice);
	}

	void DrawFilledCircle(Vector2D center, CColor color, float radius, float points) {
		std::vector<SDK::Vertex_t> vertices;
		float step = (float)M_PI * 2.0f / points;

		for (float a = 0; a < (M_PI * 2.0f); a += step)
			vertices.push_back(SDK::Vertex_t(Vector2D(radius * cosf(a) + center.x, radius * sinf(a) + center.y)));

		DrawTexturedPoly((int)points, vertices.data(), color);
	}

	void DrawT(int X, int Y, CColor Color, int Font, bool Center, const char* _Input, ...)
	{
		int apple = 0;
		/* set up buffer */
		char Buffer[2048] = { '\0' };

		/* set up varargs*/
		va_list Args;

		va_start(Args, _Input);
		vsprintf_s(Buffer, _Input, Args);
		va_end(Args);

		size_t Size = strlen(Buffer) + 1;

		/* set up widebuffer*/
		wchar_t* WideBuffer = new wchar_t[Size];

		/* char -> wchar */
		mbstowcs_s(0, WideBuffer, Size, Buffer, Size - 1);

		/* check center */
		int Width = 0, Height = 0;

		if (Center)
		{
			INTERFACES::Surface->GetTextSize(Font, WideBuffer, Width, Height);
		}

		/* call and draw*/
		INTERFACES::Surface->DrawSetTextColor(Color.RGBA[0], Color.RGBA[1], Color.RGBA[2], Color.RGBA[3]);
		INTERFACES::Surface->DrawSetTextFont(Font);
		INTERFACES::Surface->DrawSetTextPos(X - (Width / 2), Y);
		INTERFACES::Surface->DrawPrintText(WideBuffer, wcslen(WideBuffer));

		return;
	}

	RECT GetTextSizeRect(DWORD font, const char* text)
	{
		size_t origsize = strlen(text) + 1;
		const size_t newsize = 100;
		size_t convertedChars = 0;
		wchar_t wcstring[newsize];
		mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

		RECT rect; int x, y;
		INTERFACES::Surface->GetTextSize(font, wcstring, x, y);
		rect.left = x; rect.bottom = y;
		rect.right = x;
		return rect;
	}

	Vector2D GetMousePosition()
	{
		POINT mousePosition;
		GetCursorPos(&mousePosition);
		ScreenToClient(FindWindow(0, "Counter-Strike: Global Offensive"), &mousePosition);
		return { static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y) };
	}

	bool MouseInRegion(int x, int y, int x2, int y2) {
		if (GetMousePosition().x > x && GetMousePosition().y > y && GetMousePosition().x < x2 + x && GetMousePosition().y < y2 + y)
			return true;
		return false;
	}
}
