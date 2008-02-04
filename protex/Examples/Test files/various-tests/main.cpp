//#include <math.h>
//#include "Core.h"
//
//using namespace std;
//
//#define showd(x) printf(#x"=%d\n",x)
//#define showf(x) printf(#x"=%f\n",x)
//float myfRandom()
//{
//	return (float)myRandom(1) / 65536.0f;
//}
//
//
//
//int main()
//{
//SetSeedValue(5412);
//	double sum = 0;
//	int i;
//	for(i =0; i < 1000;i++)
//		sum +=myfRandom();
//	sum /=i;
//showf(sum);
//    return 0;
//}

#include <windows.h>
#include <winuser.h>
#include <stdio.h>
#define showsizeof(x) printf(#x"=%f\n",sizeof(x))
int main()
{
	showsizeof(LRESULT);
	showsizeof(UINT);
	showsizeof(LPARAM);
	showsizeof(HPARAM);
}
