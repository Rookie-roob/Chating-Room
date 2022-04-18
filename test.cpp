#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <bitset>
using namespace std;

// void preFixSum2D(int **image,int w,int h,int **prefix)
// {
//     for(int i=0;i<w;++i)
//     {
//         for(int j=0;j<h;++j)
//         {
//             if(i==0 && j==0)
//                 prefix[i][j]=image[i][j];
//             else if(i==0)
//                 prefix[i][j]=image[i][j]+prefix[i][j-1];
//             else if(j==0)
//                 prefix[i][j]=image[i][j]+prefix[i-1][j];
//             else 
//                 prefix[i][j]=image[i][j]+prefix[i-1][j]+prefix[i][j-1]-prefix[i-1][j-1];
//         }
//     }
// }

// int  mean(int **prefix,int w,int h,int r,int t)
// {
//     int res=0;
//     int aver=0,size=(2*r+1)*(2*r+1);
//     for(int i=r;i<w-r;i++)
//     {
//         for(int j=r;j<h-r;j++)
//         {
//             aver=(prefix[i+r][j+r]+prefix[i-r][j-r]-prefix[i-r][j+r]-prefix[i+r][j-r])/size;
//             if(aver<t) 
//                 res++;
//         }
//     }
//     return res;
// }

// typedef void (*Func)(int);
// void outputInDifferentSystem(int x, Func f)
// {
// 	f(x);
// }
// void outputInDifferentSystem2(int x, void (*f)(int x))
// {
// 	f(x);
// }
 
// //the functions you define with the type void (int);
// void operation1(int x)
// {
// 	cout << x << endl;
// }
// void operation2(int x)
// {
// 	cout << bitset<sizeof(x)*8>(x) << endl;
// }
// void operation3(int x)
// {
// 	cout << oct << x << endl;
// }
// void operation4(int x)
// {
// 	cout << hex << x << endl;
// }
  
int main()
{
	int val = 5;
	return 0;
}
