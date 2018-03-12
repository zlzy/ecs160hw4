//
//  main.c
//  maxTweeter
//
//  Created by Tianqi Zhu on 3/7/18.
//  Copyright © 2018 Tianqi Zhu. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char * name;
    int  frequency;
} myMap;

char *strtok_single (char * str, char const * delims)
/*String token later will be called to split , */
{
    static char  * src = NULL;
    char  *  p,  * ret = 0;
    
    if (str != NULL)
        src = str;
    
    if (src == NULL)
        return NULL;
    
    if ((p = strpbrk (src, delims)) != NULL)
    {
        *p  = 0;
        ret = src;
        src = ++p;
        
    }
    else if (*src)
    {
        ret = src;
        src = NULL;
    }
    
    return ret;
}

void printTopTen(myMap data[], int curSize)
/*print top ten tweeters, if there is less than 10 ppl, print them all*/
{
    for(int i = 0; i < 10; i++)
    {
        int largest = 0;
        int largestPos = -1;
        
        for(int j = 0; j < curSize; j++)
            if(data[j].frequency > largest)
            {
                largest = data[j].frequency;
                largestPos = j;
            }
        
        if(largestPos == -1)
        {
            printf("Less than 10 users in the file\n");
            break;
        }
        

        printf("%s",data[largestPos].name);
        printf(":");
        printf("%d\n",data[largestPos].frequency);
        data[largestPos].frequency = 0;
    }
    
    return;
}

int main(int argc, char ** argv) {
    
    //Check that argc == 2
    if(argc!= 2)
    {
        printf("Invalid Input Format\n");
        return 0;
    }

    char const* const fileName =
    //"/Users/tianqizhu/Desktop/ecs160海外/maxTweeter/maxTweeter/cl-tweets-short1.csv";
    argv[1];
    

    
    FILE* file = fopen(fileName, "r");
    
//    if((strlen(argv[1])<= 4) ||(strlen(argv[1]) > 4 && strcmp(argv[1] + strlen(argv[1]) - 4, ".csv")))
//    {
//        printf("Invalid Input Format\n");
//        return 0;
//    }
//
    
    
    if (file == NULL)
    /*Check the fopen result*/
    {
        printf("Invalid Input Format\n");
        return 0;
    }
    
    if (NULL != file)//Check for empty
    {
        fseek (file, 0, SEEK_END);
        long size = ftell(file);
        
        if (0 == size)
        {
            printf("Invalid Input Format\n");
            return 0;
        }
        fseek(file, 0, SEEK_SET);
    }
    

    
    char line[1024];/*the buffer size doesn't matter here */
    int readName = 1;
    int readLine = 20000;
    int count = 0;
    int namePos = -1;
    int curSize = 0;
    int blockCount = 0;
    myMap data[20000];

    
    while (fgets(line, sizeof(line), file))
    {
        if(count < readName)
        /*try to find "name" in first row*/
        {
            
            int nameCount = 0;
            char *ch;
            ch = strtok_single(line, ",");
            while (ch != NULL)
            {
                
                if(strlen(ch) >= 1 && !strcmp(ch + strlen(ch) - 1, "\n"))//handle last word in a line
                    ch = strtok_single(ch, "\n");
                
                
                blockCount++;
                if(strcmp("\"name\"", ch) == 0)
                {
                    namePos = nameCount;
                    //break;//Find col NAME in the first row of a csv
                }
                else
                    nameCount++;
                
                ch = strtok_single(NULL, ",");
            }
            
            if(namePos == -1)//Can't find col NAME in the first row of a csv
            {
                printf("Invalid Input Format\n");
                return 0;
            }
            
            count++;
        }
        else if(count < readLine)
        {
            char *ch;
            ch = strtok_single(line, ",");
            int temp =0;
            int curBlockCount = 0;
            while (ch != NULL)
            {
                
                if(strlen(ch) >= 1 && !strcmp(ch + strlen(ch) - 1, "\n"))
                {
                    ch = strtok_single(ch, "\n");
                }
                   
                curBlockCount++;
                if(temp==namePos)
                {
                    int flag = 0;// flag indicates if we need to dynamically allocate a new string in our struct map
                    
                    for(int i =0; i < curSize; i++)
                        if(!strcmp(data[i].name,ch))
                        {
                            data[i].frequency++;
                            flag = 1;
                            break;
                        }

                    if(!flag)
                    {
                        data[curSize].name= malloc(strlen(ch)+1);
                        data[curSize].frequency = 1;
                        strcpy(data[curSize].name,ch);
                        curSize++;
                    }
                    
                   // break;
                }
                ch = strtok_single(NULL, ",");
                temp++;
            }
            
            if(curBlockCount!=blockCount)// Invalid line
            {
                printf("Invalid Input Format\n");
                return 0;
            }
            count++;
        }
    }
    /* may check feof here to make a difference between eof and io failure -- network
     timeout for instance */
    
    fclose(file);
    
    printTopTen(data,curSize);
    
    for(int i =0; i < curSize; i++)
        free(data[i].name);
    
    return 0;
}
