#include "pch.h"
#include "CX_Array.h"

CX_Array::CX_Array(void)
{
}

CX_Array::~CX_Array(void)
{
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