/**************************************************/
/*NAME: MERGESORT USING MULTITHREAD		  */
/*DATE: 3RD MAY 2019  				  */
/*SUBJECT: OPERATING SYSTEM			  */
/*DESCRIPTION: IMPLEMENTED MERGESORT USING 2      */
/*THREADS NOTING DOWN THE TIME FOR DIFFERENT 	  */
/*NUMBER OF ELEMENTS				  */ 
/**************************************************/

#include <stdio.h>
#include <stdlib.h>
//LIBRARY FOR IMPLEMENTING THREADS
#include <pthread.h>
//LIBRARY FOR CALCULATING TIME TAKEN
#include <time.h>
#define NOOFTHREADS 2
//GLOBAL ARRAY USED
#define MAX_SIZE 1024

int array[MAX_SIZE];

typedef struct node {
    int i; // ARRAY START
    int j; // ARRAY END
} NODE;

//MERGING THE TWO SUB-ARRAYS
void merge(int i, int j)
{
    int mid = (i+j)/2;
    int k   = i;
    int l   = mid+1;
	int m	= 0;
    int newArray[j-i+1];

    while(k <= mid && l <= j) 
	{
        if (array[k] > array[l])
            newArray[m++] = array[l++];
        else                    
            newArray[m++] = array[k++];
    }

	//FILL THE REMAINING ELEMENTS
    while(k <= mid) 
		newArray[m++] = array[k++];

    while(l <= j) 
		newArray[m++] = array[l++];

	//COPY TO THE ORIGNAL ARRAY    
    for (k = 0; k < (j-i+1) ; k++)
		array[i+k] = newArray[k];
}

void * mergesort(void *a)
{
    NODE *p = (NODE *)a;
    NODE n1, n2;
    int mid = (p->i+p->j)/2;
    pthread_t tid1, tid2;

    n1.i = p->i;
    n1.j = mid;

    n2.i = mid+1;
    n2.j = p->j;

    if (p->i >= p->j)
		return 0;

    int ret;
    
	//CREATING FIRST THREAD	
    ret = pthread_create(&tid1, NULL, mergesort, &n1);
	// (THREAD NAME, ATTRIBUTE, STARTROUTINE, ARGUMENTS)
	if (ret) // RET!=0 IS ERROR
	{
		printf("%d %s - unable to create thread - ret - %d\n", __LINE__, __FUNCTION__, ret);    
		exit(1);
    }

	//CREATING SECOND THREAD	
    ret = pthread_create(&tid2, NULL, mergesort, &n2);
    if (ret) 
	{
		printf("%d %s - unable to create thread - ret - %d\n", __LINE__, __FUNCTION__, ret);    
		exit(1);
    }
    
	//JOIN - WAITS FOR THREAD TERMINATION
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    merge(p->i, p->j);
    //TERMINATING THE CALLING THREADS
    pthread_exit(NULL);
}


int main()
{
    int i;
    NODE n;
	srand(time(NULL));
	printf("Unsorted Array:\n");
	
	//CREATING UNSORTED ARRAY OF SIZE MAX_SIZE
	for (i=0;i<MAX_SIZE;i++)
	{
    	array[i]=rand()%100;
		printf("%d ",array[i]);
	}
	printf("\n");

    n.i = 0;
    n.j = MAX_SIZE-1;
    
    //MAIN THREAD CREATED
    pthread_t tid;

	//STARTING TIME AND ENDIN TIME TO CALCULATE TIME TAKEN
	clock_t startingTime, endingTime;
	//START TIME
	startingTime=clock();
    
	int ret; 
    ret=pthread_create(&tid, NULL, mergesort, &n);
    if (ret) 
	{
		printf("%d %s - unable to create thread - ret - %d\n", __LINE__,__FUNCTION__, ret);    
		exit(1);
    }
    pthread_join(tid, NULL);
	//END TIME
	endingTime = clock();
	
	printf("Sorted Array:\n");
    for (i = 0; i < MAX_SIZE; i++)
		printf ("%d ", array[i]);
    printf ("\n");

	//SUBTRACT STARTIME FROM ENDTIME TO GET TIME TAKEN
	printf("Time taken: %f\n",(endingTime - startingTime)/(double)CLOCKS_PER_SEC);
    //pthread_exit(NULL);
    return 0;
}
