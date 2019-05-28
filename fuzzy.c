#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "fuzzy.h"

#define K 1  //k Aprroximate Match(k diffs tolerated)

//=====================================================
// function: readFile
//=====================================================
int readFile(char *dir, char** text)
{
   FILE    *fp;
   long    numBytes;
 
   fp = fopen(dir, "r");
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
   ////printf("text:\n%s\n", *text);
   printf("total:%ld bytes\n", numBytes);
   return 0;
}


//=====================================================
// function: findExact
// find the exact match
//=====================================================
int findExact(char *text, char *pattern, int lenText, int lenPattern, int offset)
{
   int found = 0;
   int lastPos = 0;
   while(text)
   {
      char *ptr = strstr(text, pattern);
      if (ptr)
      {
         found = 1;
         //printf("a match found at index:%lu\n", ptr-text+lastPos);
         printf("[%lu-%lu]:%s,", ptr-text+lastPos, ptr-text+lenPattern-offset+lastPos, pattern);
      }  
      lastPos += ptr-text+1;
      if(ptr)
      {
         text = ptr + 1;
      }
      else
      {
         //printf("no match found...\n");
         break;
      }

   }

   return 0;
}

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

#if 1
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

   ///////// TODO: move those two printings to fuction
   // print DP matrix before processing
   printf("original DP matrix:\n");
   for (int i = 0; i < m+1; i++)
   {
      for (int j = 0; j < n+1; j++)
      {
         printf("%d,",DP[i][j]);
      }
      printf("\n");
   }

   // fill the DP matrix
   // first col:pattern, first row:text
   for (int j = 1; j < n+1; j++)
   {
      for (int i = 1; i < m+1; i++)
      {
         if (pattern[i-1] == text[j-1]) //cur char match
         {
            DP[i][j] = findMin(DP[i-1][j-1], DP[i-1][j]+1, DP[i][j-1]+1);
         }
         else  // cur char differ
         {
            DP[i][j] = findMin(DP[i-1][j-1]+1, DP[i-1][j]+1, DP[i][j-1]+1);
         }
      }
   }

   // print DP matrix after filled
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
#endif

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
      ptr = ptr + 1;
   }
   printf("\n");
}

#if 1
//=====================================================
// function: backtracking
// find out the route to reach the "succeed point"
// "succeed point":element less or equal than K
// return length of moves
//=====================================================

int backtracking(int** DP, Move* moves, int j, int m, int n, int k)
{
   
   int step = 0;  //can not know total num of step ahead, so record reversely only
   int i = m;

   while(i > 0 && j > 0) //last(first before reverse) step reaches border(first col or first row)
   {
      // case 1: DP[i][j] came diagonally, cur char match
      if (DP[i][j] == DP[i-1][j-1]) 
      {
         printf("-------- diagonal same -------\n");
         printf("the %d-th character of pattern matched\n", i);
         printf("i=%d,j=%d,DP[i][j]=%d,DP[i-1][j-1]=%d,\n", i,j,DP[i][j],DP[i-1][j-1]);
         moves[step++] = diag_same;
         i--; j--;
      }
      else
      {
         int minParent = findMin(DP[i-1][j-1], DP[i][j-1], DP[i-1][j]);
         int diffPos;
         
         ////printf("minParent+1=%d, DP[i-1][j-1]=%d\n", minParent+1, DP[i-1][j-1]);

         // case 2: DP[i][j] came diagonally, cur char differ
         if (minParent == DP[i-1][j-1]) // diagonal, cur char mismatched --> substitution
         {
            printf("-------- diagonal diff -------\n");
            printf("i=%d,j=%d,DP[i][j]=%d,DP[i-1][j-1]=%d,\n", i,j,DP[i][j],DP[i-1][j-1]);
            moves[step++] = diag_diff;
            i--; j--;
         } 

         // case 3: DP[i][j] came horizontally, text has one extra char --> pattern insertion will match
         else if (minParent == DP[i][j-1])
         {
            printf("-------- horizontal --------\n");
            printf("i=%d,j=%d,DP[i][j]=%d,DP[i][j-1]=%d,\n", i,j,DP[i][j],DP[i][j-1]);
            moves[step++] = hori;
            j--;
         }

         // case 4: DP[i][j] came vertically, pattern has one more char --> pattern deletion will match 
         else if (minParent == DP[i-1][j])
         {
            printf("-------- vertical --------\n");
            printf("i=%d,j=%d,DP[i][j]=%d,DP[i-1][j]=%d,\n", i,j,DP[i][j],DP[i-1][j]);
            moves[step++] = vert;
            i--;
         } 
         else
         {
            printf("route calculation error\n");
         }
         
      }   
   }
   printf("end of backtracking....\n");
   return step; //length of moves
}

//=====================================================
// function: analyzeDP
// find out the "succeed point"
// "succeed point":element less or equal than K
//=====================================================
int analyzeDP(char* text, int** DP, int m, int n, int k)
{
   printf("enter analyzeDP\n");
   if (DP == NULL)
   {
      printf("DP NULL");
      return -1;
   }
  
   // examine the last row for values less or equal to K, mark as "succeed point"
   for (int j = 0; j < n+1; j++)
   {
      if (DP[m][j] <= k)
      {
         printf("#################################################\n");
         printf("a fuzzy match with k=%d tolerance found...at text idx=%d\n", K, j);
         Move *moves = calloc(m+n+1, sizeof(Move)); //keep record of moves.max num of steps:m+n+1
         int numSteps = backtracking(DP, moves, j, m, n, k); // find out the route to reach the "succeed point"
         printf("moves-len:%d,moves:", numSteps);

         for (int i = numSteps; i >= 0; i--)
         {
            switch(moves[i])
            {
               case 0:
                  printf("diag_same,");
                  break;
               case 1:
                  printf("diag_diff,");
                  break;
               case 2:
                  printf("hori,");
                  break;
               case 3:
                  printf("vert,");
                  break;
               default:
                  break;
            }
            
         }
         printf("\n");

         printing(text, j-numSteps+1, j);
      }
   }
   return 0;

}
#endif

#if 1
//=====================================================
// function: getDeletion
//=====================================================
int getDeletion(char *pattern, int lenPattern, char *text, int lenText)
{
   printf("deletion:");
   int offset = 2;
   char *deleted = calloc(lenPattern-1, sizeof(char));
   for(int i = 0; i < lenPattern; i++)
   {
      for (int j = 0; j < i; j++)
      {
         deleted[j] = pattern[j];
      }
      for (int j = i+1; j < lenPattern; j++)
      {
         deleted[j-1] = pattern[j];
      }
      //printf("deleted:%s\n", deleted);
      findExact(text, deleted, lenText, lenPattern, offset);
   }
   printf("\n");
   return 0;
}
#endif

#if 1
//=====================================================
// function: getTransposition
//=====================================================
int getTransposition(char *pattern, int lenPattern, char *text, int lenText)
{
   printf("transposition:");
   int offset = 1;
   char *transposed = calloc(lenPattern, sizeof(char));
   for(int i = 0; i < lenPattern; i++)
   {
      for (int j = 0; j < lenPattern; j++)
      {
         strcpy(transposed, pattern);
         char tmp = transposed[j];
         transposed[j] = transposed[i];
         transposed[i] = tmp;
         //printf("transposed:%s\n", transposed);
         findExact(text, transposed, lenText, lenPattern, offset);
      }
      
   }
   printf("\n");
   return 0;
}
#endif

#if 1
//=====================================================
// function: getSubstitution
//=====================================================
int getSubstitution(char *pattern, int lenPattern, char *text, int lenText)
{
   printf("substitution:");
   int offset = 1;
   char *substituted = calloc(lenPattern, sizeof(char));
   for(int i = 0; i < lenPattern; i++)
   {
      for (int k = 0; k < 26; k++)
      {
         for (int j = 0; j < i; j++)
         {
            substituted[j] = pattern[j];
         }
         substituted[i] = 'a' + k;
         for (int j = i+1; j < lenPattern; j++)
         {
            substituted[j] = pattern[j];
         }
         //printf("substituted:%s\n", substituted);
         findExact(text, substituted, lenText, lenPattern, offset);
      }
      
   }
   printf("\n");
   return 0;
}
#endif

#if 1
//=====================================================
// function: getInsertion
//=====================================================
int getInsertion(char *pattern, int lenPattern, char *text, int lenText)
{
   printf("insertion:");
   int offset = 0;
   char *inserted = calloc(lenPattern, sizeof(char));
   for(int i = 0; i < lenPattern+1; i++)
   {
      for (int k = 0; k < 26; k++)
      {
         for (int j = 0; j < i; j++)
         {
            inserted[j] = pattern[j];
         }
         inserted[i] = 'a' + k;
         for (int j = i+1; j < lenPattern+1; j++)
         {
            inserted[j] = pattern[j-1];
         }
         //printf("inserted:%s\n", inserted);
         findExact(text, inserted, lenText, lenPattern, offset);
      }
      
   }
   printf("\n");
   return 0;
}
#endif

//=====================================================
// function: readDict
//=====================================================
int readDict(char **dict)
{
   int i = 0;
   FILE * fp;
   char * pattern = calloc(30, sizeof(char));
   char *text;
   size_t len = 0;
   ssize_t read;
   fp = fopen("./dictionary.txt", "r");
   if (fp == NULL)
   {
      exit(EXIT_FAILURE);
   }

   readFile("./small.txt", &text); //attention!!DO NOT pass "text"!remember &
   printf("text:\n%s\n", text);

//======================================================
// please enable the following block to test "ware" case
//======================================================
   #if 1
   pattern = "ware";
   int numRow = strlen(pattern);
   int numCol = strlen(text);

   //DP = kAprroximateMatch(pattern, text, numRow, numCol);
   //analyzeDP(text, DP, numRow, numCol, K);

   getDeletion(pattern, numRow, text, numCol);
   getTransposition(pattern, numRow, text, numCol);
   getSubstitution(pattern, numRow, text, numCol);
   getInsertion(pattern, numRow, text, numCol);
   #endif
//======================================================


//======================================================
// please enable the following block to test dictionary case
//======================================================
   #if 0
   while ((read = getline(&pattern, &len, fp)) != -1)
   {
      printf("%s", pattern);

      int numRow = strlen(pattern);
      int numCol = strlen(text);

      //DP = kAprroximateMatch(pattern, text, numRow, numCol);
      //analyzeDP(text, DP, numRow, numCol, K);

      getDeletion(pattern, numRow, text, numCol);
      getTransposition(pattern, numRow, text, numCol);
      getSubstitution(pattern, numRow, text, numCol);
      getInsertion(pattern, numRow, text, numCol);
   }
   #endif
//======================================================

   return 0;
}


//=====================================================
// main function
//=====================================================

int main()
{
   
   char **dict;
   int **DP;
   char *pattern;
   
   readDict(dict);
   

   //free(text);
}



