#ifndef UTILS_H
#define UTILS_H
#pragma once

#include <math.h>

class CUtils
{
public:
	CUtils(void) {}
	~CUtils(void) {}
/*
	static UINT maximum(UINT *arr, UINT len) {
		UINT max = arr[0];
		for(UINT i = 0; i < len; ++i){
			if(max < arr[i]){
				max = arr[i]; 
			}
		}
		return max;
	 }
	
	static BOOL isPrime(UINT x){
		if(x == 2)  return TRUE;
		if(x % 2 == 0) return FALSE;
		for(UINT i = 3; i <= (int)sqrt((float)x); i += 2){
			if(x % i == 0) return FALSE; 
		}
		return TRUE;
	 }

	static UINT lcm(UINT *a, UINT len) {
		UINT lcm = 1;
		UINT max =  maximum(a, len);
		UINT i, j, temp;
		for(i = 2; i <= max; ++i) {
			if(isPrime(i)) {
				temp = 1;
				for(j = 0; j < len; ++j) {
					while(a[j] % temp == 0) {
						temp *= i;
					}
				}
				temp /= i;
				lcm *= temp;
			}
		}
		return lcm;
	}
*/
	/* 数组的最小公倍数;*/  
	static UINT GetCommonMultipler(UINT *nSampleRate, UINT nSampleNum)   //lhx 08-1-18
	{
		UINT i = 0;
		UINT nFirst = nSampleRate[0];
		for (i = 0 ; i < nSampleNum ; i++ )
		{
			
			nFirst = CommonMultiple(nSampleRate[i],nFirst );
		}
		return nFirst; 
	}

	/* 数组的最大公约数 */
	static UINT GetCommonDivider(UINT *nSampleRate, UINT nSampleNum) //lhx 08-1-18
	{
		UINT i = 0;
		UINT nFirst = nSampleRate[0];
		for ( i = 0 ; i < nSampleNum ; i++ )
		{
			
			nFirst = CommonDivisor(nSampleRate[i],nFirst );
		}
		return nFirst; 

	}

	/* 最小公倍数;*/  
	static UINT CommonMultiple(UINT m, UINT n) //lhx 08-1-18
	{
		UINT   temp=m*n/CommonDivisor(m,n);  
		return   temp;  

	}

	/* 最大公约 */
	static UINT CommonDivisor(UINT m, UINT n)//lhx 08-1-18
	{
		if(n==0)  {
			return  m;  
		} else {
			return CommonDivisor(n,m%n);  
		}
	}

	static UINT32 modeSize(UINT32 word) { 
		UINT32 size; 
		size = word * 2; 

		if (size % 4) { 
			size = size / 4 + 1; 
		} else {
			size /= 4; 
		}
		return size * 4; 
	} 

};

#endif