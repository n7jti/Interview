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

// Krypto.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


class CMathOps
{
typedef int (CMathOps::*OP)(int, int);
public:
	CMathOps()
	{
		m_aOp[0] = &CMathOps::add;
		m_aOp[1] = &CMathOps::subtract;
		m_aOp[2] = &CMathOps::multiply;
		m_aOp[3] = &CMathOps::divide;
		m_aOp[4] = &CMathOps::pow;

		m_aOpSym[0] = L'+';
		m_aOpSym[1] = L'-';
		m_aOpSym[2] = L'*';
		m_aOpSym[3] = L'/';
		m_aOpSym[4] = L'^';

		ZeroMemory(m_aIndex, sizeof(m_aIndex));
	}

	int Calculate(int aIn[4])
	{
		int iRetVal;
		iRetVal = (this->*m_aOp[m_aIndex[0]])(aIn[0], aIn[1]); 
		iRetVal = (this->*m_aOp[m_aIndex[1]])(iRetVal, aIn[2]);
		iRetVal = (this->*m_aOp[m_aIndex[2]])(iRetVal, aIn[3]);
		// wprintf(L"%02d %c %02d %c %02d %c %02d = %d\n", aIn[0], m_aOpSym[m_aIndex[0]],aIn[1],m_aOpSym[m_aIndex[1]], aIn[2],m_aOpSym[m_aIndex[2]], aIn[3], iRetVal);
		return iRetVal;
	}

	int Solve(int aIn[4], int iOut)
	{
		int a[4];
		CopyMemory(a, aIn, sizeof(a));
		int iCount = 0;

		for(int i = 0; i < 24; i++)
		{
			for(int j = 0; j < 125; j++)
			{
				if (iOut == Calculate(a))
				{
					 // we found a solution!
					iCount++;
					wprintf(L"%02d %c %02d %c %02d %c %02d = %d\n", a[0], m_aOpSym[m_aIndex[0]],a[1],m_aOpSym[m_aIndex[1]], a[2],m_aOpSym[m_aIndex[2]], a[3], iOut);
				}
				Combine();
			}
			CopyMemory(a, aIn, sizeof(a));
			Permute(a,4,i);
		}
		return iCount;
	}

private:
	int add(int a, int b)
	{
		return a + b;
	}

	int subtract(int a, int b)
	{
		return a - b;
	}

	int multiply(int a , int b)
	{
		return a * b;
	}

	int divide(int a, int b)
	{
		return a / b;
	}

	int pow(int a, int b)
	{
		int retval = 1; 

		if(0 != b)
		{
			for(int i = 0 ; i < b; i++)
				retval *= a;
		}

		return retval;
	}

	int Combine()
	{
		const int count = 3;
		const int iMax = 5;

		m_aIndex[0]++;
		int i = 0;
		for(i = 0; i < count - 1; i++)
		{
			m_aIndex[i+1] += m_aIndex[i] / iMax;
			m_aIndex[i] = m_aIndex[i] % iMax;
		}
		m_aIndex[i] = m_aIndex[i] % iMax; 
		return 0;
	}

	int Permute(int* aIn, int cIn, int iPermutation)
	{
		int iFactorial = 1;

		//compute (aIn-1)!
		for(int i = 2; i < cIn; i++)
		{
			iFactorial *= i;
		}
		
		int iTemp1 = 0;
		int iTemp2 = 0;
		for(int i = 1; i < cIn; i++)
		{
			iTemp1 = (iPermutation / iFactorial) % (cIn + 1 - i);
			iTemp2 = aIn[i + iTemp1 -1];
			for (int j =  i + iTemp1; j > i; j--)
			{
				aIn[j-1] = aIn[j-2]; // shfit the chain to the right
			}
			aIn[i-1] = iTemp2;
			iFactorial /= (cIn - i);

		}
		return 0;
	}

	OP m_aOp[5];  // aprray of function pointers
	WCHAR m_aOpSym[5];
	int m_aIndex[3];
};

class CDeck
{
public:
	CDeck()
	{
		ZeroMemory(m_iDeck, sizeof(m_iDeck));
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 13; j++)
			{
				m_iDeck[(i) * 13 + j] = j+1;
			}
		}
		
		m_iNextCard = 0;
	}

	void Shuffle()
	{
		int i;
		int iIndex;
		int iSwap;
		for( i = 0; i < 52; i++)
		{
			iIndex = rand() % 52;
			iSwap = m_iDeck[i];
			m_iDeck[i] = m_iDeck[iIndex];
			m_iDeck[iIndex] = iSwap;
		}
		m_iNextCard = 0;
	}

	int Deal()
	{
		int iCard = m_iDeck[m_iNextCard];
		m_iNextCard++;
		if(m_iNextCard >= 52)
		{
			Shuffle();
		}
		return iCard;
	}

private:
	int m_iDeck[52];
	int m_iNextCard;
};


int _tmain(int argc, _TCHAR* argv[])
{
	int a[4];
	int b;
	int count;
	CDeck dDeck;
	dDeck.Shuffle();

	for(int i = 0 ; i < 5; i++)
	{
		a[0]=dDeck.Deal();
		a[1]=dDeck.Deal();
		a[2]=dDeck.Deal();
		a[3]=dDeck.Deal();
		b = dDeck.Deal();
	
		wprintf(L"%d %d %d %d %d\n\n", a[0],a[1],a[2],a[3], b);
	    CMathOps mOps;
		count = mOps.Solve(a,b);
		wprintf(L"There were %d solutions\n\n", count);
	}

	return 0;
}

