#include "draw.h"
void CDraw::Setup(LPDIRECT3DDEVICE9 device)
{
    CDraw::pDevice = device;
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
