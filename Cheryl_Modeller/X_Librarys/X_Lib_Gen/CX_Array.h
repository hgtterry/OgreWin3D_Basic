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

#pragma once

struct tag_Array
{
	int ItemsAllocated;		// number of items currently allocated
	int ItemSize;
	void* Items;			// array of data
};

typedef struct tag_Array Array;

#define Array_GetSize(a) ((a)->ItemsAllocated)
#define Array_GetItemSize(a) ((a)->ItemSize)
#define Array_ItemPtr(a,i) ((void *)(((uintptr_t)((a)->Items))+((i)*(a)->ItemSize)))
#define Array_PutAt(a,i,d,s) (memcpy (Array_ItemPtr((a),(i)),(d),(s)))
#define Array_InsertAt(a,i,d,s) \
		(memmove(Array_ItemPtr((a),(i)+1),Array_ItemPtr((a),(i)),((a)->ItemsAllocated-(i)-1)*(a)->ItemSize), \
		 memcpy(Array_ItemPtr((a),(i)),(d),(s)))
#define Array_DeleteAt(a,i) \
		(memcpy (Array_ItemPtr((a),(i)),Array_ItemPtr((a),(i+1)),((a)->ItemsAllocated-(i)-1)*(a)->ItemSize))

class CX_Array
{
public:
	CX_Array(void);
	~CX_Array(void);

	char* GetVersion();

	signed int Array_Init(Array* pArray, int InitialSize, int ItemSize);
	void Array_Uninit(Array* pArray);
	Array* Array_Create(int InitialSize, int ItemSize);
	void Array_Destroy(Array** ppArray);
	int Array_Resize(Array* pArray, int NewSize);
};

