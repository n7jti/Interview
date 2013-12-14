// Copyright 2013 Alan Ludwig
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http ://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// LinkedList.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

struct SList
{
	int iData;
	SList* pNext;
};

SList* AllocateList()
{
	SList* pList = NULL; 
	SList* pCur = NULL;
	pList = new SList;
	pList->iData = 0;
	pList->pNext = NULL;
	pCur = pList;

	for(int i = 0; i < 10; i++)
	{
		pCur->pNext = new SList;
		pCur = pCur->pNext;
		pCur->iData = rand() % 100;
		pCur->pNext = NULL;

	}

	return pList; 
}

void FreeList(SList* pList)
{
	SList* pCur = NULL;
	while(pList)
	{
		pCur = pList;
		pList = pList->pNext;
		delete pCur;
		pCur = NULL;
	}
}

void ReverseList (SList** ppList)
{
	SList* pCur = NULL;
	SList* pRev = NULL;
	SList* pFor = *ppList;

	while(pFor)
	{
		pCur = pFor;
		pFor = pFor->pNext;
		pCur->pNext = pRev;
		pRev = pCur;
		pCur = NULL;
	}

	*ppList = pRev;
}

SList* SortList(SList** ppList)
{
	SList* pOut = NULL;
	SList* pIn = *ppList;
	SList* pCur = NULL;
	SList* pPrev = NULL;
	
	
	while(pIn)
	{
		if(!pOut)
		{
			pOut = pIn;
			pIn = pIn->pNext;
			pOut->pNext = NULL;
		}
		else if(pIn->iData < pOut->iData)
		{
			pCur = pOut;
			pOut = pIn;
			pIn = pIn->pNext;
			pOut->pNext = pCur;
		}
		else
		{
			pPrev = pOut;
			pCur = pPrev->pNext;
			while(pCur)
			{
				if(pIn->iData < pCur->iData)
				{
					pPrev->pNext = pIn;
					pIn = pIn->pNext;
					pPrev->pNext->pNext = pCur;
					break;
				}
				else
				{
					pPrev = pCur;
					pCur = pCur->pNext; 
				}
			}

			if (NULL == pPrev->pNext)
			{
				pPrev->pNext = pIn;
				pIn = pIn->pNext;
				pPrev->pNext->pNext = NULL;
			}
		}

	}

	*ppList = NULL;
	return pOut;
}

void DumpList(SList* pList)
{
	wprintf(L"LIST: %p\n",pList);
	while(pList)
	{
		wprintf(L"\t%p: %d\n", pList, pList->iData);
		pList = pList->pNext;
	}
}

bool flipCoin()
{
	return rand() % 2 ? true : false;
}


void ShuffleList(SList** ppList)
{
	SList* pLeft;
	SList* pRight;
	SList* pDeck;
	SList* pCur;

	pDeck = *ppList;
	//seven shuffles is enough to get good randomization
	for(int i=1; i <= 7; i++)
	{
		pLeft = pDeck;
		pCur = pDeck;
		pRight = pDeck;
		pDeck = NULL;
		
		int count = 0;
		while(pCur)
		{
			pCur = pCur->pNext;
			count++;
		}

		count = count / 2;

		while(count > 1)
		{
			count--;
			pRight = pRight->pNext;
		}

		pCur = pRight;
		pRight = pRight->pNext;
		pCur->pNext = NULL;

		while(pLeft && pRight)
		{
			if( true == flipCoin() )
			{
				//Get from	 left
				pCur = pLeft; 
				pLeft = pLeft->pNext;
				pCur->pNext = pDeck;
				pDeck = pCur;
				pCur = NULL;

			}
			else
			{
				//Get from right
				pCur = pRight;
				pRight = pRight->pNext;
				pCur->pNext = pDeck;
				pDeck = pCur;
				pCur = NULL;

			}
		}

		while(pLeft)
		{
			pCur = pLeft; 
			pLeft = pLeft->pNext;
			pCur->pNext = pDeck;
			pDeck = pCur;
			pCur = NULL;
		}

		while (pRight)
		{
			pCur = pRight;
			pRight = pRight->pNext;
			pCur->pNext = pDeck;
			pDeck = pCur;
			pCur = NULL;
		}
		
	}

	
	*ppList = pDeck;
	return;
}



SList* MergeList(SList** ppTo, SList** ppFrom)
{
	SList* pMerge;
	SList* pMergeCur;
	pMerge = NULL;
	pMergeCur = pMerge;
	SList* pTo = *ppTo;
	SList* pFrom = *ppFrom;

	if (pTo && pFrom)
	{
	    // both have data
		if(pTo->iData <= pFrom->iData)
		{
			//take from pToCur
			pMerge = pTo;
			pTo = pTo->pNext;
		}
		else
		{
			//take from the other side
			pMerge = pFrom;
			pFrom = pFrom->pNext;
		}
		pMergeCur = pMerge;
	}
	else
	{
		if(!pTo)
		{
			pMerge = pFrom;
		}
		else
		{
			pMerge = pTo;
		}
	}
	
	while (pTo && pFrom)
	{
		if(pTo->iData <= pFrom->iData)
		{
			//take from pToCur
			pMergeCur->pNext = pTo;
			pTo = pTo->pNext;
			pMergeCur = pMergeCur->pNext;
		}
		else
		{
			//take from the other side
			pMergeCur->pNext = pFrom;
			pFrom = pFrom->pNext;
			pMergeCur = pMergeCur->pNext;
		}
	}

	//Pick-up any leftover data
	if (pFrom)
	{
		pMergeCur->pNext = pFrom;
	}

	if (pTo)
	{
		pMergeCur->pNext = pTo;
	}

	*ppTo = NULL;
	*ppFrom = NULL;
	return pMerge;
}


int wmain(int argc, wchar_t* argv[])
{
	// seed the random number generator
	srand( (unsigned)time( NULL ) );

	SList* pHead = AllocateList();
	SList* pHead2 = AllocateList();
	DumpList(pHead);
	ReverseList(&pHead);
	DumpList(pHead);
	ShuffleList(&pHead);
	DumpList(pHead);
	pHead = SortList(&pHead);
	DumpList(pHead);
	pHead = SortList(&pHead);
	DumpList(pHead);
	pHead2 = SortList(&pHead2);
	DumpList(pHead2);
	pHead = MergeList(&pHead, &pHead2);
	DumpList(pHead);
	FreeList(pHead);
	FreeList(pHead2);
	pHead = NULL;

	return 0;
}
