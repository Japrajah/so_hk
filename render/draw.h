#pragma once
#include <d3d9.h>
#include <d3dx9.h>


class CDraw
{
public:
	inline static LPDIRECT3DDEVICE9 pDevice =0 ;
   static void Setup(LPDIRECT3DDEVICE9 device);
   static void DrawLine(int bx, int by, int bw, D3DCOLOR COLOR);
  
};


