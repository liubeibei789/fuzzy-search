#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

int readFile(char** buffer)
{
   FILE    *fp;
   long    numBytes;
 
   fp = fopen("./small.txt", "r");
   if(fp == NULL)
      return 1;

   // Get the number of bytes
   fseek(fp, 0L, SEEK_END);  //fseek:Reposition stream position indicator
   numBytes = ftell(fp); //ftell:current pos in stream

   // reset the file position indicator to the beginning of the file
   fseek(fp, 0L, SEEK_SET);

   *buffer = (char*)calloc(numBytes, sizeof(char));	
   if(*buffer == NULL)
      return 1;
 
   fread(*buffer, sizeof(char), numBytes, fp);
   fclose(fp);
   //printf("buffer:\n%s\n", *buffer);
   printf("total:%ld bytes\n", numBytes);
   return 0;
}

#if 0
int findExact(char *fileBuf, char *phrase)
{
   char *ptr = strstr(fileBuf, phrase);
   if (ptr)
   {
   	printf("an exact match found at index:%lu\n", ptr-fileBuf);
   }
   
   return 0;
}
#endif

//=====================================================
// function: findMin
// find the smallest one among the three, 
// return 0,1,2 respectively
//=====================================================

int findMin(int a, int b, int c)
{
   if (a <= b && a <= c) return a;
   if (b <= a && b <= c) return b;
   if (c <= a && c <= b) return c;
   return 0;
}

//=====================================================
// function: kAprroximateMatch
// fuzzy searching "pattern" in "text", 
// with k differences tolerated.
// m:pattern length, n:text length
// return DP matrix
//=====================================================

int** kAprroximateMatch(char* pattern, char* text, int m, int n, int k)
{
	printf("enter kAprroximateMatch\n");
   printf("looking for %s\n", pattern);

	// DP matrix memory allocation 
	int** DP = calloc(m, sizeof(int*));
   for (int i = 0; i < n; i++)
   {
   	DP[i] = calloc(n, sizeof(int));
   }
   
   // pattern is empty, 0 difference with text t1...tj
   for (int j = 0; j < n; j++)
   {
   	DP[0][j] = 0;
   }
   // pattern p1...pi, i difference with empty text
   for (int i = 0; i < m; i++)
   {
   	DP[i][0] = i;
   }

   // print before processing
   printf("original DP matrix:\n");
   for (int i = 0; i < m; i++)
   {
   	for (int j = 0; j < n; j++)
   	{
   		printf("%d,",DP[i][j]);
   	}
   	printf("\n");
   }

   // fill the matrix

   for (int j = 1; j < n; j++)
   {
   	for (int i = 1; i < m; i++)
   	{
         if (pattern[i] == text[j])
         {
            DP[i][j] = findMin(DP[i-1][j-1], DP[i-1][j]+1, DP[i][j-1]+1);
            //printf("111:%d=%d,%d,%d\n",DP[i][j],DP[i-1][j-1], DP[i-1][j]+1, DP[i][j-1]+1);
         }
         else
         {
         	DP[i][j] = findMin(DP[i-1][j-1]+1, DP[i-1][j]+1, DP[i][j-1]+1);
            //printf("222:%d=%d,%d,%d\n",DP[i][j],DP[i-1][j-1]+1, DP[i-1][j]+1, DP[i][j-1]+1);
         }
         //printf("pattern[i]=%c,text[j]=%c,DP[%d][%d]=%d\n", pattern[i],text[j],i,j,DP[i][j]);

   	}
   	if (DP[m-1][j] <= k)
      {
         int len = strlen(pattern);
         printf("found...j=%d,", j);
         printf("len=%d,", len);
         for(int p = 0; p < len; p++)
         {
            printf("%c,", text[j+p]);
         }
         printf("\n");
      	//return j;
      }
   }

   // print after filled
   printf("edited DP matrix:\n");
   for (int i = 0; i < m; i++)
   {
      for (int j = 0; j < n; j++)
      {
         printf("%d,",DP[i][j]);
      }
      printf("\n");
   }

   return DP;
}

//int backtracking()

int main()
{
   char *buffer;
   int **DP;
   readFile(&buffer); //attention!!DO NOT pass "buffer"!remember &
   char *str = "ware";
   printf("buffer:\n%s\n", buffer);
   ////findExact(buffer, str);

   DP = kAprroximateMatch(str, buffer, 4, 7, 1);


   free(buffer);
}



