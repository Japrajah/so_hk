#pragma once
struct Vector3
{
	float x, y, z;
}; 
struct Vector2
{
	float x, y, z;
};
struct Position3D : public Vector3 { float x2, y2, z2; };
struct Matrix4x4
{
	float m[4][4];
};