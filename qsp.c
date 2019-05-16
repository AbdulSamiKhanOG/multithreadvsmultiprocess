/*****************************************************/
/*NAME: QUICKSORT USING MULTIPROCESS		     */
/*DATE: 3RD MAY 2019				     */
/*SUBJECT: OPERATING SYSTEM			     */
/*DESCRIPTION: IMPLEMENTED QUICKSORT USING 2 PROCESS */
/*NOTING DOWN THE TIME FOR DIFFERENT NUMBER OF       */
/*ELEMENTS 					     */
/*****************************************************/
#include<stdio.h>
#include<sys/wait.h> 
#include<stdlib.h>
#include<unistd.h>

//LIBRARIES FOR MULTIPROCESSING
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/ipc.h>

//LIBRARY FOR THREAD IMPLEMENTATION
#include <time.h>

//SWAPPING BY REFERENCE
void swap(int *x,int *y)
{
   int temp=*x;
   *x=*y;
   *y=temp;
}


// CREATING RANDOM ARRAY
void TakingInput(int arr[], int len)
{
	printf("Unsorted Array: \n");    
	int i;
    for (i=0; i<len; i++)
	{
		arr[i] = rand()%100;
		printf ("%d ",arr[i]);
	}
	printf("\n");	
    return;
}

int partition(int array[], int start, int end){
	//PIVOT SELECTED AS THE LAST VALUE
    int pivot = array[end];
    int index = (start - 1);
 
    for (int i = start; i <= end - 1; i++)
	{
        if (array[i] <= pivot)
		{
            index++;
            swap(&array[index], &array[i]);
        }
    }
    swap(&array[index + 1], &array[end]);
    return (index + 1);
}

void quickSort(int array[], int start, int end)
{
    int leftPid,rightPid;
    
    if (start < end)
	{
        int partIndex = partition(array, start, end);
        //LEFTCHILD CREATED
        leftPid=fork();
        if(leftPid<0)
		{
			perror("Left Child Process not Created!");
			_exit(-1);
        }
        else  if(leftPid==0)
		{
	        quickSort(array, start, partIndex - 1);
        }
		else
		{
			//RIGHT CHILD CREATED
			rightPid=fork();
			if(rightPid<0)
			{
			    perror("Right Child Process not Created!");
			    _exit(-1);
	        }
			else if(rightPid==0)
			    quickSort(array, partIndex + 1, end);
        }
   	int returnStatus=0;
    waitpid(leftPid,&returnStatus,0);
    waitpid(rightPid,&returnStatus,0);
    }
}
 
void display(int array[], int size)
{
    for (int i=0; i < size; i++)
        printf("%d ", array[i]);
}
 

int main()
{
    int shmid, size;
    
	printf("Enter number of elements: ");
	scanf("%d",&size);
	
    int *shm_array;
    size_t shm_size = sizeof(int)*size;
    
    //CREATING THE SHARED MEMORY BLOCK
    if((shmid = shmget(IPC_PRIVATE,shm_size,IPC_CREAT | 0666))<0)
	{
        perror("shmget");
        _exit(-1);
    }
    
    //ATTACHING THE SHARED MEMORY TO THE GIVEN DATA SPACE
    if((shm_array = shmat(shmid,NULL,0)) == (int *)-1)
	{
        perror("shmat");
        _exit(-1);
    }
    
    //FILLIGN ARRAY WITH RANDOM ELEMENT
    srand(time(NULL));
    TakingInput(shm_array, size);
    
    clock_t startingTime, endingTime;
    //START TIME
	startingTime = clock();
	
	quickSort(shm_array, 0, size-1);
    printf("\nSorted array: ");

    display(shm_array, size);
    //END TIME
	endingTime = clock();
	
	//DETACTCHING THE SHARED MEMORY BLOCK
    if(shmdt(shm_array)==-1)
	{
        _exit(-1);
    }
    //DESTROYING THE SHARED MEMORY BLOCK
    if(shmctl(shmid,IPC_RMID,NULL)==-1)
	{
        _exit(-1);
    } 
    //STARTING AND ENDING TIMES OF PROCESSES
	printf("Time taken: %f  %f\n",endingTime/(double)CLOCKS_PER_SEC, startingTime/(double)CLOCKS_PER_SEC);
    return 0;
}
