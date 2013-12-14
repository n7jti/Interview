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

// bits.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int CountBits(DWORD dwSource)
{
	// executes 32 times in a loop
	int iCount = 0;
	for(int i = 0; i < 32; i++)
	{
		if(dwSource & 1) iCount++;
		dwSource = dwSource >> 1;
	}
	return iCount;
}

int CountBits2(DWORD dwSource)
{
	// An improviement over one as the 
	// loop will terminiate early if there
	// are no more bits set
	int iCount = 0;
	while(dwSource)
	{
		iCount += (dwSource & 1);
		dwSource = dwSource >> 1;
	}
	return iCount;
}

int CountBits3(DWORD dwSource)
{	// An improvement again, as this one
	// will only loop as many times as there
	// are bits set.  As the algorithym iterates it 
	// continues to clear the bottom set bit until finally
	// they are all clear
	int iCount = 0;
	while(dwSource)
	{
		iCount++;
		dwSource = dwSource & (dwSource - 1);
	}
	return iCount;
}

int CountBits4(DWORD dwSource)
{
	// 0000 = 0
	// 0001 = 1
	// 0010 = 1
	// 0011 = 2
	// 0100 = 1
	// 0101 = 2
	// 0110 = 2
	// 0111 = 3
	// 1000 = 1
	// 1001 = 2
	// 1010 = 2
	// 1011 = 3
	// 1100 = 2
	// 1101 = 3
	// 1110 = 3
	// 1101 = 4
	static char t[16] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4};
	int iCount = 0;

	iCount = t[dwSource & 0x0f] +
			t[(dwSource >> 4) & 0x0f] +
			t[(dwSource >> 8) & 0x0f] +
			t[(dwSource >> 12)& 0x0f] +
			t[(dwSource >> 16) & 0x0f] +
			t[(dwSource >> 20) & 0x0f] +
			t[(dwSource >> 24) & 0x0f] +
			t[(dwSource >> 28) & 0x0f];

	return iCount;
}



int CountBits5(DWORD dwSource)
{
	// Mask off the lower two bits per nybble and then replace them with the count of bits in the pair
	// 0x5 = 0101
	dwSource = (dwSource & 0x55555555) + (0x55555555 & (dwSource >> 1));

	// now we just need to sum the pairs  in each nybble
	// 0x3 = 0011;
	dwSource = (dwSource & 0x33333333) + (0x33333333 & (dwSource >> 2));

	// Now we need to sum the nybbles in each byte
	dwSource = (dwSource & 0x0f0f0f0f) + (0x0f0f0f0f & (dwSource >> 4));

	// Now we just need to sum the bytes in each word
	dwSource = (dwSource & 0x00ff00ff) + (0x00ff00ff & (dwSource >> 8));

	// now we just need to sum the Words in each Byte
	dwSource = (dwSource & 0x0000ffff) + (0x0000ffff & (dwSource >> 16));

	return dwSource;
}

int CountBits6(DWORD dwSource)
{
	//  in   out
	// 0 00  00 0
	// 1 01  01 1
	// 2 10  01 1
	// 3 11  10 2
	//
	// So, taking the value of the pair and subtracting 1 when the high-bit is set gives
	// the right answer 

	// This expression divides the original number into paris of bits where each
	// pair now contains the count of bits that was previously in that pair
	dwSource = dwSource - ((dwSource >> 1) & 0x55555555);

	// now we just need to sum the pairs  in each nybble
	// 0x3 = 0011;
	dwSource = (dwSource & 0x33333333) + (0x33333333 & (dwSource >> 2));

	// Now we need to sum the nybbles in each byte
	dwSource = (dwSource + (dwSource >> 4)) & 0x0f0f0f0f;

	// Now we just need to sum the bytes in each word
	dwSource = dwSource + (dwSource >> 8);

	// now we just need to sum the Words in each Byte
	dwSource = dwSource + (dwSource >> 16);

	dwSource = dwSource & 0x3F;

	return dwSource;
}

int Add (int A, int B) 
{
	int Carry = (A & B) << 1;
	int Sum = A ^ B;

	while(Carry)
	{
		A = Carry;
		B = Sum;

		Carry = (A & B) << 1;
		Sum = A ^ B;
	}

	return Sum;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int iCount = 0;
	//DWORD dwSource = 0xFFFFFFFF;
	//iCount = CountBits(dwSource);
		
	int iSum = 0x0a0b0c0d;
	for (int i = 0; i < 100; i++)
	{

		iSum = Add(iSum, i);
		wprintf(L"%d: \t%d \t%d \t%d \t%d \t%d \t%d \t%d\n", i, iSum, CountBits(iSum), CountBits2(iSum), CountBits3(iSum), CountBits4(iSum), CountBits5(iSum), CountBits6(iSum));
	}
	return iCount;
}

