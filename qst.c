/*****************************************************/
/*NAME: QUICKSORT USING MULTITHREAD       	     */
/*DATE: 3RD MAY 2019                                 */
/*SUBJECT: OPERATING SYSTEM			     */
/*DESCRIPTION: IMPLEMENTED QUICKSORT USING 2 THREADS */
/*NOTING DOWN THE TIME FOR DIFFERENT NUMBER OF       */
/*ELEMENTS                                           */
/*****************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<time.h> 
#include<pthread.h>
#define siz 510
void swap(int* a, int* b);
void * quickSort(void *vptr);
int partition (int arr[], int low, int high);
void printArray(int arr[], int size);

int arr[siz];
pthread_t thread1,thread2;
struct parameter
{
    	int high;
	int low;
};

int main()
{
	int j;  
	pthread_t thread3;
	struct parameter data1;
	double cpu_time_used;
	clock_t start, end;
        int arr_size = sizeof(arr)/sizeof(arr[0]);
	srand(time(NULL));
	printf("Unsorted: ");
        for(j=0;j<siz;j++)
	{
		arr[j]=(rand()%100);
		printf("%d ",arr[j]);	
	}
	data1.high=siz-1;
	data1.low=0;
	start=clock();
        pthread_create(&thread3,0, quickSort,&data1);
        pthread_join(thread3,NULL);
		
       end=clock();
       cpu_time_used = (double)( (end - start))/ CLOCKS_PER_SEC; 
       printf("\nTime Taken => %f",cpu_time_used); 
	printf("\nSorted: ");
	printArray(arr,siz);
    return 0;
}
void swap(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}


void printArray(int arr[], int size)
{
    int i;
    for (i=0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}


void * quickSort(void *vptr )
{
    struct parameter data;
	  data = *(struct parameter*)vptr;

	  struct parameter data2;
	  data2 = *(struct parameter*)vptr;

   	 struct parameter data3;
	  data3 = *(struct parameter*)vptr;
       

    if (data.low < data.high)
    {
       
        int pi = partition(arr, data.low, data.high);
 
        
        data2.low=data.low;
        data2.high=pi-1;
         
        data3.low=pi+1;
        data3.high=data.high;

        pthread_create(&thread1,0, quickSort,&data2);
	pthread_create(&thread2,0, quickSort,&data3);

        pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);
    }
}
 


int partition (int arr1[], int low, int high)
{
    int pivot = arr1[high];    
    int i = (low - 1);
    for (int j = low; j <= high- 1; j++)
    {
        if (arr1[j] <= pivot)
        {	         
	    i++;   
            swap(&arr1[i], &arr1[j]);
        }
    }
    swap(&arr1[i + 1], &arr1[high]);
    return (i + 1);
}

