#ifndef CUCUSTOMVERTEX_H
#define CUCUSTOMVERTEX_H

#include "stdafx.h"

namespace cuCustomVertex
{
	//Position
	typedef struct Position
	{
	public:
		Position() : X(0), Y(0), Z(0) {}
		Position(float x, float y, float z)
			: X(x), Y(y), Z(z) {}
		float X, Y, Z;
	} Position;

	//Position and color
	typedef struct PositionColor
	{
	public:
		PositionColor() : X(0), Y(0), Z(0), Color(0) {}
		PositionColor(float x, float y, float z, DWORD color)
			: X(x), Y(y), Z(z), Color(color) {}
		float X, Y, Z;
		DWORD Color;
	} PositionColor;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	Summary: Position and color
	* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	typedef struct PositionTextured
	{
	public:
		PositionTextured() : X(0), Y(0), Z(0), Tu(0), Tv(0) {}
		PositionTextured(float x, float y, float z, float tu, float tv)
			: X(x), Y(y), Z(z), Tu(tu), Tv(tv) {}
		float X, Y, Z;
		float Tu, Tv;
	} PositionTextured;

	typedef struct Position2Textured
	{
	public:
		Position2Textured() : X(0), Y(0), Z(0), Tu1(0), Tv1(0), Tu2(0), Tv2(0) {}
		Position2Textured(float x, float y, float z, float tu1, float tv1, float tu2, float tv2)
			: X(x), Y(y), Z(z), Tu1(tu1), Tv1(tv1), Tu2(tu2), Tv2(tv2) {}
		float X, Y, Z;
		float Tu1, Tv1;
		float Tu2, Tv2;
	} Position2Textured;

	//Position, color, texture coordinates
	typedef struct PositionColorTextured
	{
	public:
		PositionColorTextured() : X(0), Y(0), Z(0), Color(0), Tu(0), Tv(0) {}
		PositionColorTextured(float x, float y, float z, DWORD color, float tu, float tv)
			: X(x), Y(y), Z(z), Color(color), Tu(tu), Tv(tv) {}
		float X, Y, Z;
		DWORD Color;
		float Tu, Tv;
	} PositionColorTextured;

	//Position, color, texture coordinates
	typedef struct PositionNormalTextured
	{
	public:
		PositionNormalTextured() : X(0), Y(0), Z(0), Nx(0), Ny(0), Nz(0), Tu(0), Tv(0) {}
		PositionNormalTextured(float x, float y, float z, float nx, float ny, float nz, float tu, float tv)
			: X(x), Y(y), Z(z), Nx(nx), Ny(ny), Nz(nz), Tu(tu), Tv(tv) {}
		float X, Y, Z;
		float Nx, Ny, Nz;
		float Tu, Tv;
	} PositionNormalTextured;
}
#endif // !CUCUSTOMVERTEX_H
