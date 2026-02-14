/*
Copyright (c) 2024 - 2025 TMH_Software W.T.Flanigan M.Habib H.C.Flanigan

TMH_SceneBuilder

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "pch.h"
#include "CX_Array.h"

CX_Array::CX_Array(void)
{
}

CX_Array::~CX_Array(void)
{
}

// *************************************************************************
// *			GetVersion:- Terry and Hazel Flanigan 2025			 	   *
// *************************************************************************
char* CX_Array::GetVersion()
{
	return (LPSTR)" TMH_Scene_Builder :-- CX_Array [ 06-09-25 ] Build 1 ";
}

signed int CX_Array::Array_Init(Array* pArray, int InitialSize, int ItemSize)
{
	pArray->ItemSize = ItemSize;
	pArray->ItemsAllocated = 0;

	// if user requested initial size, 
	// then allocate memory for the items.
	if (InitialSize == 0)
	{
		pArray->Items = NULL;
	}
	else
	{
		pArray->Items = malloc(InitialSize * ItemSize);
		if (pArray->Items == NULL)
		{
			return false;
		}
		pArray->ItemsAllocated = InitialSize;
	}
	return true;
}

void CX_Array::Array_Uninit(Array* pArray)
{
	if (pArray->Items != NULL)
	{
		free(pArray->Items);
		pArray->Items = NULL;
	}
	pArray->ItemsAllocated = 0;
}

// *************************************************************************
// * Array_Createreate an array object with given initial size (possibly 0)*
// *************************************************************************
Array* CX_Array::Array_Create(int initialSize, int itemSize)
{
	// Allocate memory for the Array structure
	Array* pArray = static_cast<Array*>(malloc(sizeof(Array)));

	// Check if memory allocation was successful
	if (pArray != nullptr)
	{
		// Initialize the array and check for success
		if (!Array_Init(pArray, initialSize, itemSize))
		{
			// If initialization fails, destroy the array and set pointer to nullptr
			Array_Destroy(&pArray);
			pArray = nullptr; // Ensure pArray is null after destruction
		}
	}

	return pArray; // Return the pointer to the created array or nullptr
}

// Destroy an array object
void CX_Array::Array_Destroy(Array** ppArray)
{
	Array_Uninit(*ppArray);
	free(*ppArray);
	*ppArray = NULL;
}

// Resizes the array to contain NewSize elements.
// Returns new size.
int CX_Array::Array_Resize(Array* pArray, int NewSize)
{
	void* NewItems;

	NewItems = realloc(pArray->Items, (NewSize * pArray->ItemSize));

	// realloc returns NULL in two cases:
	//   1) It was unable to allocate memory
	//   2) The size parameter was 0.
	if ((NewItems != NULL) || (NewSize == 0))
	{
		pArray->Items = NewItems;
		pArray->ItemsAllocated = NewSize;
	}

	return pArray->ItemsAllocated;
}