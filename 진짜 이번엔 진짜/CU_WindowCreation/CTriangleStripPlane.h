#ifndef CTRIANGLESTRIPPLANE_H
#define CTRIANGLESTRIPPLANE_H

#include "stdafx.h"
#include "cuCustomVertex.h"

class CTriangleStripPlane
{
public:
	static void GeneratePositionNormalTextured(cuCustomVertex::PositionNormalTextured** ppVertices, int verticesAlongWidth, int verticesAlongLength);
	static void GeneratePositionTexturedWithHeight(cuCustomVertex::PositionTextured** ppVertices, int verticesAlongWidth, int verticesAlongLength, UCHAR* pHeight);
	static void GeneratePosition2TexturedWithHeight(cuCustomVertex::Position2Textured** ppVertices, int verticesAlongWidth, int verticesAlongLength, UCHAR* pHeight);
	static int GenerateIndices(int** ppIndices, int verticesAlongWidth, int verticesAlongLength);
	static int GenerateIndices(USHORT** ppIndices, int verticesAlongWidth, int verticesAlongLength);
};

#endif