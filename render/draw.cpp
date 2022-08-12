#include "draw.h"
#pragma comment (lib, "d3dx9.lib")

// pepeEZ PASTED FROM UC
void CDraw::Setup(LPDIRECT3DDEVICE9 device)
{
	if (CDraw::pDevice) return;
	CDraw::pDevice = device;
	D3DXCreateFontA(CDraw::pDevice, 14, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &CDraw::m_font);
}


void CDraw::DrawLine(int bx, int by, int bw, D3DCOLOR COLOR)
{
   
    D3DRECT rec;
    rec.x1 = bx - bw;//makes line longer/shorter going lef
    rec.y1 = by; // base y
        rec.x2 = bx + bw;//makes line longer/shorter going right
    rec.y2 = by + 1;//makes line one pixel tall
    CDraw::pDevice->Clear(1, &rec, D3DCLEAR_TARGET, COLOR, 0, 0);
}

void CDraw::Text( unsigned int x, unsigned int y, const char* Message ,D3DCOLOR color )
{	// Create a colour for the text
	RECT rct; //Font
	rct.left = x;
	rct.right = 1680;
	rct.top = y;
	rct.bottom = rct.top + 200;
	m_font->DrawTextA(NULL, Message, -1, &rct, 0, color);
}

void CDraw::TextW(unsigned int x, unsigned int y, const wchar_t* Message, D3DCOLOR color)
{	// Create a colour for the text
	RECT rct; //Font
	rct.left = x;
	rct.right = 1680;
	rct.top = y;
	rct.bottom = rct.top + 200;
	m_font->DrawTextW(NULL, Message, -1, &rct, 0, color);
}
