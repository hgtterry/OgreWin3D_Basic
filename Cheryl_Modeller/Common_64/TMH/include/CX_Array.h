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

	/*signed int Array_Init(Array* pArray, int InitialSize, int ItemSize);
	void Array_Uninit(Array* pArray);
	Array* Array_Create(int InitialSize, int ItemSize);
	void Array_Destroy(Array** ppArray);
	int Array_Resize(Array* pArray, int NewSize);*/
};

