#pragma once
class CX_Cut_Brush
{
public:
	CX_Cut_Brush(void);
	~CX_Cut_Brush(void);

	void BrushList_DoCSG(BrushList* inList, int mid, Brush_CSGCallback Callback, void* lParam);
	void Brush_SplitByFace_T(Brush* ogb, Face* sf, Brush** fb, Brush** bb);
	void Brush_SealFaces_T(Brush** b);

};

