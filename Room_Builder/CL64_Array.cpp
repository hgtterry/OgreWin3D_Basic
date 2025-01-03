#include "pch.h"
#include "CL64_App.h"
#include "CL64_Array.h"

CL64_Array::CL64_Array(void)
{
}

CL64_Array::~CL64_Array(void)
{
}

signed int CL64_Array::Array_Init(Array* pArray, int InitialSize, int ItemSize)
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

void CL64_Array::Array_Uninit(Array* pArray)
{
	assert(pArray != NULL);

	if (pArray->Items != NULL)
	{
		free(pArray->Items);
		pArray->Items = NULL;
	}
	pArray->ItemsAllocated = 0;
}


// Create an array object with given initial size (possibly 0).
Array* CL64_Array::Array_Create(int InitialSize, int ItemSize)
{
	Array* pArray;

	assert(InitialSize >= 0);
	assert(ItemSize > 0);

	pArray = (Array*)App->CL_Maths->Ram_Allocate(sizeof(Array));

	if (pArray != NULL)
	{
		if (Array_Init(pArray, InitialSize, ItemSize) == false)
		{
			Array_Destroy(&pArray);
		}
	}

	return pArray;
}

// Destroy an array object
void CL64_Array::Array_Destroy(Array** ppArray)
{
	assert(ppArray != NULL);
	assert(*ppArray != NULL);

	Array_Uninit(*ppArray);
	free(*ppArray);
	*ppArray = NULL;
}



// Resizes the array to contain NewSize elements.
// Returns new size.
int CL64_Array::Array_Resize(Array* pArray, int NewSize)
{
	void* NewItems;

	assert(pArray != NULL);
	assert(NewSize >= 0);

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

