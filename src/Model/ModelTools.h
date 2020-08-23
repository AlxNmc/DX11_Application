// Alex Niemiec 2020
// Support tools to compute bounding volumes on models
// Ported/adapted from Ed Keenan's FBX Converter code

#ifndef _ModelTools
#define _ModelTools

#include "Vect.h"

struct StandardVertex;
struct TriangleByIndex;

class ModelTools
{

public:
	static void CreateUnitBoxRepTexture(StandardVertex *&pVerts, int& nverts, TriangleByIndex *&pTriList, int& ntri);
	static void CreateUnitBoxSixFaceTexture(StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri);
	static void UnitPyramidRepTexture(StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri);
	static void CreateUnitSphere(int vslice, int hslice, StandardVertex *&pVerts, int& nverts, TriangleByIndex *&pTriList, int& ntri);
};



#endif _ModelTools