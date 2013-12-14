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

// KnightsWalk.cpp : Defines the entry point for the console application.
//
   
#include "stdafx.h"

class CBoard
{
public:
	CBoard()
	{
		ZeroMemory(m_aBoard, sizeof(m_aBoard));
		m_iCurCount = 1;
		m_aBoard[0][0] = m_iCurCount;
		m_iCurRow = 0;
		m_iCurCol = 0;
	}

	CBoard(const CBoard& source)
	{
		CopyMemory(m_aBoard, source.m_aBoard, sizeof(m_aBoard));
		m_iCurCount = source.m_iCurCount;
		m_iCurRow = source.m_iCurRow;
		m_iCurCol = source.m_iCurCol;
	}

	CBoard& operator =(const CBoard& source)
	{
		CopyMemory(m_aBoard, source.m_aBoard, sizeof(m_aBoard));
		m_iCurCount = source.m_iCurCount;
		m_iCurRow = source.m_iCurRow;
		m_iCurCol = source.m_iCurCol;
		return *this;
	}

	void Print()
	{
		wprintf(L"%I64d\n", m_llMoves);
		for (int x = 0; x < 8; x++)
		{
			for (int y = 0; y < 8; y++)
			{
				wprintf(L"\t%d", m_aBoard[x][y]);
			}
			wprintf(L"\n");
		}
		
		DWORD dwBytesWritten;
		HANDLE hFile = INVALID_HANDLE_VALUE;

		hFile = CreateFile(L"C:\\Users\\alanlu\\Documents\\out.txt", GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL,NULL);

		const DWORD FILE_WRITE_TO_END_OF_FILE = 0xFFFFFFFF;
		WCHAR szOut[256];
		OVERLAPPED o; 
		ZeroMemory(&o, sizeof(o)); 
		o.Offset = FILE_WRITE_TO_END_OF_FILE; 
		o.OffsetHigh = -1;


		wsprintf(szOut,L"%I64d\r\n", m_llMoves);
		WriteFile(hFile, szOut, static_cast<DWORD>(wcslen(szOut) * sizeof(WCHAR)), &dwBytesWritten, &o);

		for (int x = 0; x < 8; x++)
		{
			for (int y = 0; y < 8; y++)
			{
				wsprintf(szOut,L"\t%d", m_aBoard[x][y]);
				WriteFile(hFile, szOut, static_cast<DWORD>(wcslen(szOut) * sizeof(WCHAR)), &dwBytesWritten, &o);
			}
			wsprintf(szOut, L"\r\n");
			WriteFile(hFile, szOut, static_cast<DWORD>(wcslen(szOut) * sizeof(WCHAR)), &dwBytesWritten, &o);
		}

		CloseHandle(hFile);
		hFile = INVALID_HANDLE_VALUE;

		return;
	}

	BOOL Move(int m)
	{

		//
		//          7         0
		//   -*-* -2-1 -*-* -2+1 -*-*
		// 6 -1-2 -*-* -*-* -*-* -1+2 1
		//   -*-* -*-* -0-0 -*-* -*-*
		// 5 +1-2 -*-* -*-* -*-* +1+2 2
		//   -*-* +2-1 -*-* +2+1 -*-*
		//          4         3

		BOOL fRet = TRUE;
		int iNewRow = m_iCurRow + Row(m);
		int iNewCol = m_iCurCol + Col(m);
		
		if (iNewRow < 0 || iNewRow > 7 || iNewCol < 0 || iNewCol > 7 || m_aBoard[iNewRow][iNewCol] > 0)
		{
			fRet = FALSE;
		}
		else
		{
			m_iCurCount++;
			m_iCurRow = iNewRow;
			m_iCurCol = iNewCol;
			m_aBoard[m_iCurRow][m_iCurCol] = m_iCurCount;
		}

		LONGLONG llMoves = InterlockedIncrement64(&m_llMoves);
		if( 0 == llMoves % 100000000)
		{
			wprintf(L"%I64d\n",llMoves/100000000);
		}
		return fRet;
	}

	BOOL FindTourInternal(CBoard& board)
	{
		int iMove = 0;
		BOOL fRet = FALSE;
		CBoard myBoard;


		//Terminate the recursion, short-circult the result
		if(64 == board.m_iCurCount)
		{
			board.Print();
			fRet = TRUE;
			return fRet;
		}
		
		for(int i = 0; i < 8 ; i++)
		{
			myBoard = board;
			if(TRUE == myBoard.Move(i))
			{
				fRet = FindTourInternal(myBoard);
				if (TRUE == fRet)
				{
					// Comment these two statements out so that we continue to search
					board = myBoard;
					break;
				}
			}	
		}

		return fRet;
	}

	BOOL FindTour()
	{
		return FindTourInternal(*this);
	}


private:

// 00 01 02 03 04 05 06 07
// 10 11 12 13 14 15 17 17
// 20 21 22 23 24 25 26 27
// 30 31 32 33 34 35 36 37
// 40 41 42 43 44 45 46 47
// 50 51 52 53 54 55 56 57
// 60 61 62 63 64 65 66 67
// 70 71 72 73 74 75 76 77
	int m_aBoard[8][8];
	int m_iCurRow;
	int m_iCurCol;
	int m_iCurCount;
	static LONGLONG m_llMoves;

	//    R  C
	// 0 -2 +1 
	// 1 -1 +2
	// 2 +1 +2
	// 3 +2 +1
	// 4 +2 -1
	// 5 +1 -2 
	// 6 -1 -2
	// 7 -2 -1


	//       C   Sign Value
	// 0000 +1    0    0
	// 0001 +2    0    1
	// 0010 +2    0    1
	// 0011 +1    0    0
	// 0100 -1    1    0
	// 0101 -2    1    1
	// 0110 -2    1    1 
	// 0111 -1    1    0

	// Given the above truth table you can clearly see that the 0x04 bit is unset when the result is positive, and set when the result is negative
	// also, you can interpret the value as the XOR of the two low bits
	// Or, you can interpret the vlaue as the parity of the bitcount 

	// Takes in an integer, and returns the relative Column of the target square
	// Based on the idea the the function is basically the XOR of the two low bits
	int Col(int A)
	{
		int iCol = 0;
		int iSign = 0;

		iSign = A & 0x4 ? -1 : 1;

		// XOR the two low bits and add one.
		iCol = ((0x1 & A) ^ (0x1 & (A >> 1))) + 1;
		iCol *= iSign;

		return iCol; 
	}

	int Row(int A)
	{
		return Col(A + 6);
	}


	// Takes in an integer, and returns the relative Column of the target square
	// Based on the idea the the function is the parity of the bit count of the two low bits
	int Col2(int A)
	{
		int iCol = 0;
		int iSign = 0;

		iSign = A & 0x4 ? -1 : 1;

		// Find the parity of bitcount of the two low bits
		iCol = (((0x1 & A) + (0x1 & (A >> 1))) & 0x1) + 1;
		iCol *= iSign;

		return iCol; 
	}

	int Row2(int A)
	{
		return(Col2(A+6));
	}



};

LONGLONG CBoard::m_llMoves = 0;

int wmain(int argc, WCHAR* argv[])
{
	CBoard board;
	board.FindTour();
	board.Print();
	return 0;
}

