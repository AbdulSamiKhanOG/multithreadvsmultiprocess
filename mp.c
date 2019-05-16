/********************************************************/
/*NAME: MERESORT USING MULTIPROCESS			*/
/*DATE: 3RD MAY 2019					*/
/*SUBJECT: OPERATING SYSTEM  				*/
/*DESCRIPTION: IMPLEMENTED MERGESORT USING 2 PROCESSES	*/
/*NOTING DOWN THE TIME FOR DIFFERENT NUMBER OF ELEMENTS	*/
/********************************************************/

//LIBRARIES FOR MULTIPROCESSING
#include <sys/types.h> 
#include <sys/ipc.h>
#include <sys/shm.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
 
//LIBRARY FOR CALCULATING TIME TAKEN
#include <time.h>

void merge(int arr[], int low, int high1, int high2);
/* SORTING THE ARRAY USING INSERTION SORT WHEN LENGTH = 4*/
void insertionSort(int arr[], int n)
{
   int i, j, key;
   for (i = 1; i < n; i++)
   {
       key = arr[i];
       j = i-1;
       while (j >= 0 && arr[j] > key)
       {
           arr[j+1] = arr[j];
           j = j-1;
       }
       arr[j+1] = key;
   }
}

void mergeSort(int arr[], int low, int high)
{
    int i, len=(high-low+1);
    if (len<5)
    {
        insertionSort(arr+low, len);
        return;
    }
    
    //TWO PROCESS DEFINED
    pid_t lpid,rpid;
    
    //PROCESS CREATED
    lpid = fork();	
    if (lpid<0)
    {
        //LCHILD PROCESS NOT CREATED
        perror("Left Child Process not created\n");
        _exit(-1);
    }
    else if (lpid==0)
    {
		//LCHILD PROCESS CREATED
        mergeSort(arr,low,low+len/2-1);
        _exit(0);
    }
    else
    {
    	//PROCESS CREATED
        rpid = fork();
        if (rpid<0)
        {
            //RCHILD PROCESS NOT CREATED
            perror("Right Child Process not created\n");
            _exit(-1);
        }
        else if(rpid==0)
        {
        	//RCHILD PROCESS CREATED
            mergeSort(arr,low+len/2,high);
            _exit(0);
        }
    }
 
    int status;
 
    //WAIT FOR CHILD PROCESSES TO END
    waitpid(lpid, &status, 0);
    waitpid(rpid, &status, 0);
 
    // MERGE THE SORTED SUBARRAYS
    merge(arr, low, low+len/2-1, high);
}

//MERGING THE TWO SUB-ARRAYS
void merge(int arr[], int low, int high1, int high2)
{
    int size=high2-low+1;
    int i=low; 
    int k=high1+1;
    int m=0;
    int sortedArray[size];

    while (i <= high1 && k <= high2)
    {
        if (arr[i] < arr[k])
            sortedArray[m++] = arr[i++];
        else if (arr[k] < arr[i])
            sortedArray[m++]=arr[k++];
        else if (arr[i] == arr[k])
        {
            sortedArray[m++] = arr[i++];
            sortedArray[m++] = arr[k++];
        }
    }
 
 	//FILL THE REMAINING ELEMENTS
    while (i <= high1)
        sortedArray[m++] = arr[i++];
 
    while (k <= high2)
        sortedArray[m++] = arr[k++];
        
 	//COPY TO THE ORIGNAL ARRAY 
    int arr_count = low;
    for (i = 0; i < size; i++,low++)
        arr[low] = sortedArray[i];
}

//FILLING RANDOM VALUES IN THE ARRAY
void TakingInput(int arr[], int len)
{
	//printf("Unsorted Array: \n");    
	int i;
    for (i=0; i<len; i++)
	{
		arr[i] = rand()%100;
	//	printf ("%d ",arr[i]);
	}	
    return;
}

int main()
{
	int z=0;
	for(z=2;z<1030;z=z*2)
{
    
	int shmid;
    key_t key = IPC_PRIVATE;
    int *shm_array;
    int length;
 
	// printf("Enter No of elements of Array:");
	// scanf("%d",&length); 
	length=z;
    
	// CALCULATE THE SEGMENT LENGTH
    size_t SHM_SIZE = sizeof(int)*length;
 
    // CREATING SEGMENT
    if ((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0)
    {
        perror("shmget");
        _exit(1);
    }
 
    // ATTACH SEGMENT TO OUR DATA SPACE
    if ((shm_array = shmat(shmid, NULL, 0)) == (int *) -1)
    {
        perror("shmat");
        _exit(1);
    }
 
    // CREATE A RANDOM ARRAY
    srand(time(NULL));
    TakingInput(shm_array, length);
	printf ("\n");
	
   //STARTING TIME AND ENDIN TIME TO CALCULATE TIME TAKEN
    clock_t startingTime, endingTime;
	//START TIME
	startingTime=clock();
	
    mergeSort(shm_array, 0, length-1);
    //END TIME
	endingTime = clock();
	
	//printf("Sorted Array:\n");
	//for (int i=0;i<length;i++)
	//printf ("%d ",shm_array[i]);
	//printf ("\n");
 
   //DETACH FROM THE SHARED MEMORY
    if (shmdt(shm_array) == -1)
    {
        perror("shmdt");
        _exit(1);
    }
 
   //DELETE SHARED MEMORY
   //IPCRMID -> DESTROY SEGMENT
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        perror("shmctl");
        _exit(1);
    }
	//SUBTRACT STARTIME FROM ENDTIME TO GET TIME TAKEN
    printf("Time taken %d: %f\n",z,(endingTime - startingTime)/(double)CLOCKS_PER_SEC);
}
    return 0;
}
