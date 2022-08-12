#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9core.h>

class CDraw
{
public:
	inline static LPDIRECT3DDEVICE9 pDevice =0 ;
	inline static  LPD3DXFONT m_font = 0;
	static void Setup(LPDIRECT3DDEVICE9 device);
	static  void DrawLine(int bx, int by, int bw, D3DCOLOR COLOR);
	static  void Text(unsigned int x, unsigned int y, const char* Message, D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255));
	static  void TextW(unsigned int x, unsigned int y, const wchar_t* Message, D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255));

};


