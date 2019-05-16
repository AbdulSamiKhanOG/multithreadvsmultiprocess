
/*********************************************************/
/*NAME: MATRIX MULTIPLICATION USING MULTITHREAD	         */
/*DATE: 10TH MAY 2019					 */
/*SUBJECT: OPERATING SYSTEM				 */
/*DESCRIPTION: IMPLEMENTED MATRIX MULT. USING 2 THREADS  */
/*NOTING DOWN THE TIME FOR DIFFERENT NUMBER OF ELEMENTS  */
/*********************************************************/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define max 10

int matA[max][max];
int matB[max][max];
int matC[max][max];

int dividing_variable=0;

void* multi(void* arg)
{
    int core = dividing_variable++;
 
    
    for (int i = core * max / 4; i < (core + 1) * max / 4; i++) 
        for (int j = 0; j < max; j++) 
            for (int k = 0; k < max; k++) 
                matC[i][j] += matA[i][k] * matB[k][j];
}


int main(){
 /*start:   
    printf("Enter Size of Metrix X:X");
    scanf("%d",max);
    if(max<2){
     printf("\n\nError Enter again");
 goto start;
}
*/
   pthread_t threads[max];
   clock_t start, end;
   double cpu_time_used;
    for (int i = 0; i < max; i++) {
        for (int j = 0; j < max; j++) {
            matA[i][j] = rand() % 10;
            matB[i][j] = rand() % 10;
        }
    }

    
    printf("\n");
         printf("Matrix A\n");
    for (int i = 0; i < max; i++) {
        for (int j = 0; j < max; j++) 
            printf("%d  ", matA[i][j]);;
        printf("\n");
    }
 
   
    printf("\n");
         printf("Matrix B\n");
    for (int i = 0; i < max; i++) {
        for (int j = 0; j < max; j++) 
            printf("%d  ", matB[i][j]);        
        printf("\n");
    }
     start=clock();
     for (int i = 0; i < max; i++) {
        int* p;
        pthread_create(&threads[i], NULL, multi, (void*)(p));
    }


     for (int i = 0; i < max; i++) 
        pthread_join(threads[i], NULL); 

     end=clock();
     printf("\n");
         printf("mul of Matrix A and B\n");
    for (int i = 0; i < max; i++) {
        for (int j = 0; j < max; j++) 
            printf("%d  ", matC[i][j]);       
        printf("\n");
    }

    cpu_time_used = (double)( (end - start))/ CLOCKS_PER_SEC; 
		printf("\ncpu time => %f",cpu_time_used); 
    return 0;





}


