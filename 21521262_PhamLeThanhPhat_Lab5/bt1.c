// UIT
// MSSV: 21521262
// bt1.c
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
sem_t sem1, sem2;
int sells = 0, products;

void *processA()
{
    while (1)
    {
        sem_wait(&sem1);
        sells++;
        sem_post(&sem2);
        printf("sells: sells = %d, products = %d, %d \n", sells, products, products - sells);
    }
}

void *processB()
{
    while (1)
    {
        sem_wait(&sem2);
        products++;
        sem_post(&sem1);
        printf("products: sells = %d, products = %d, %d \n", sells, products, products - sells);
    }
}
int main()
{
    sem_init(&sem1, 0, 0);
    sem_init(&sem2, 0, 72);
    pthread_t pA, pB;
    pthread_create(&pA, NULL, processA, NULL);
    pthread_create(&pB, NULL, processB, NULL);
    pthread_join(pA, NULL);
    pthread_join(pB, NULL);
    return 0;
}
/*
sem1 đảm bảo rằng (sells<=products), (khởi tạo sells = 0).
sem2 đảm bảo rằng (products <= sells + 72), (khởi tạo sells = 72).
Khi mới bắt đầu tiến trình A sẽ bị chặn lại bởi lệnh sem_wait(&sem1).
Khi ProcessB chạy, products++ và sem2-- (bởi sem_wait(&sem2)) và sem1++ sau mỗi vòng lặp cho đến khi sem2 = 0.
Sau đó ProcessA được chạy (khi giá trị của sem1 > 0). 
Khi đó sells++ và sem2++ (bởi hàm sem_post(&sem2)) và sem1--  (bởi sem_wait(&sem1)) cho đến khi sem1 = 0.
Hai Tiến trình lúc này sẽ luân phiên nhau tăng product và sell đồng thời cũng sẽ đảm bảo được điều kiện đề bài cho.
*/
