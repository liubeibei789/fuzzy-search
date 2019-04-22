#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "fuzzy.h"

#define K 1  //k Aprroximate Match(k diffs tolerated)

int readFile(char** text)
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

   *text = (char*)calloc(numBytes, sizeof(char));	
   if(*text == NULL)
      return 1;
 
   fread(*text, sizeof(char), numBytes, fp);
   fclose(fp);
   //printf("text:\n%s\n", *text);
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
// m: length of pattern, n: length of text
// return DP matrix
//=====================================================

int** kAprroximateMatch(char* pattern, char* text, int m, int n)
{
	printf("enter kAprroximateMatch\n");
   printf("looking for %s\n", pattern);

	// DP matrix memory allocation 
	int** DP = calloc(m+1, sizeof(int*));
   for (int i = 0; i < m+1; i++)
   {
   	DP[i] = calloc(n+1, sizeof(int));
   }
   
   // pattern is empty, 0 difference with text t1...tj
   for (int j = 0; j < n+1; j++)
   {
   	DP[0][j] = 0;
   }
   // pattern p1...pi, i difference with empty text
   for (int i = 0; i < m+1; i++)
   {
   	DP[i][0] = i;
   }

   // print before processing
   printf("original DP matrix:\n");
   for (int i = 0; i < m+1; i++)
   {
   	for (int j = 0; j < n+1; j++)
   	{
   		printf("%d,",DP[i][j]);
   	}
   	printf("\n");
   }

   // fill the matrix

   for (int j = 1; j < n+1; j++)
   {
   	for (int i = 1; i < m+1; i++)
   	{
         if (pattern[i-1] == text[j-1])
         {
            //printf("i=%d,pattern[i]=%c;j=%d,text[j]=%c\n",i-1,pattern[i-1],j-1,text[j-1]);
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
      /*
   	if (DP[m][j] <= k)
      {
         int len = strlen(pattern);
         printf("found...j=%d,", j);
         printf("len=%d,", len);
         
         for(int p = len; p > 0; p--)
         {
            printf("%c,", text[j-p+1]);
         }
         printf("\n");
         
      	//return j;
      }
      */
   }

   // print after filled
   printf("edited DP matrix:\n");
   for (int i = 0; i < m+1; i++)
   {
      for (int j = 0; j < n+1; j++)
      {
         printf("%d,",DP[i][j]);
      }
      printf("\n");
   }

   return DP;
}

//=====================================================
// function: printing
//=====================================================

void printing(char* text, int start, int end)
{
   char *ptr = text;
   ptr += start;
   printf("%d-%d:", start, end);
   for (int i = start; i <= end; i++)
   {
      printf("%c,", *ptr);
   }
   printf("\n");
}

//=====================================================
// function: backtracking
// find out the route to reach the "succeed point"
// "succeed point":element less or equal than K
//=====================================================

#if 0
int backtracking(char* text, int** DP, int m, int n, int k)
{
   printf("enter backtracking\n");
   if (DP == NULL)
   {
      printf("DP NULL");
      return -1;
   }

   Move moves[m+n+1]; //keep record of moves.max num of steps:m+n+1
   int step = 0;  //can not know total num of step ahead, so record reversely only

   // examine the last row for values less or equal to K
   for (int j = 0; j < n+1; j++)
   {
      if (DP[m][j] <= k)
      {
         printf("found...idx=%d\n", j);
         int jOriginal = j;
         int i = m;
         while(i >= 0 && j >= 0) //last(first before reverse) step reaches border
         {
            if (DP[i][j] == DP[i-1][j-1]) //DP[m][j] came diagonally, cur char matched
            {
               printf("the %d-th character of pattern matched", i);
               moves[step++] = diag_same;
            }
            else
            {
               int minParent = findMin(DP[i-1][j-1],DP[i][j-1],DP[i-1][j]);
               int diffPos;
               
               if (minParent+1 == DP[i-1][j-1]) // diagonal, cur char mismatched --> substitution
               {
                  moves[step++] = diag_diff;
                  if (1 == k)
                  {
                     diffPos = j;
                     printing(text, diffPos, jOriginal);
                  }
               } 
               else if (minParent+1 == DP[i][j-1])  // horizontal --> insertion
               {
                  moves[step++] = hori;
                  if (1 == k)
                  {
                     diffPos = j;
                     printing(text, diffPos, jOriginal);
                  }
               }
               else if (minParent+1 == DP[i-1][j]) // vertical --> deletion
               {
                  moves[step++] = vert;
                  if (1 == k)
                  {
                     diffPos = j+1;
                     printing(text, diffPos, jOriginal);
                  }
               } 
               else
               {
                  printf("route calculation error\n");
               }
               
            }

         }
      }
   }


   return 0;
}
#endif

int main()
{
   char *text;
   int **DP;
   readFile(&text); //attention!!DO NOT pass "text"!remember &
   char *pattern = "ware";
   printf("text:\n%s\n", text);
   ////findExact(text, pattern);

   int numRow = strlen(pattern);
   int numCol = strlen(text);

   DP = kAprroximateMatch(pattern, text, numRow, numCol);
   //backtracking(text, DP, numRow, numCol, K);

   free(text);
}



