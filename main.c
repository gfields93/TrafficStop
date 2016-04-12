/*
//
//  main.c
//  TrafficProblem
//
//  Created by
//  Glen Fields
//  on 11/17/15.
//
*/
#define _REENTRANT
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#define stringlen 100
#define Light 5
#define Car 2
int numOfCars;
sem_t * CarLimit;
sem_t * CarNum;
sem_t * ClearN;
sem_t * ClearS;
sem_t * ClearE;
sem_t * ClearW;

void CreateSemaphores(void){
    CarNum = sem_open("/CarNum", O_CREAT | O_EXCL, 0666, 1);
    if (CarNum == SEM_FAILED) {
        sem_close(CarNum);
        sem_unlink("/CarNum");
        CarNum = sem_open("/CarNum", O_CREAT, 0666, 1);
    }
    CarLimit = sem_open("/CarLimit", O_CREAT | O_EXCL, 0666, 2);
    if (CarLimit == SEM_FAILED) {
        sem_close(CarLimit);
        sem_unlink("/CarLimit");
        CarLimit = sem_open("/CarLimit", O_CREAT, 0666, 2);
    }
    ClearN = sem_open("/ClearN", O_CREAT | O_EXCL, 0666, 0);
    if (ClearN == SEM_FAILED) {
        sem_close(ClearN);
        sem_unlink("/ClearN");
        ClearN = sem_open("/ClearN", O_CREAT, 0666, 0);
    }
    ClearS = sem_open("/ClearS", O_CREAT | O_EXCL, 0666, 0);
    if (ClearS == SEM_FAILED) {
        sem_close(ClearS);
        sem_unlink("/ClearS");
        ClearS = sem_open("/ClearS", O_CREAT, 0666, 0);
    }
    ClearE = sem_open("/ClearE", O_CREAT | O_EXCL, 0666, 0);
    if (ClearE == SEM_FAILED) {
        sem_close(ClearE);
        sem_unlink("/ClearE");
        ClearE = sem_open("/ClearE", O_CREAT, 0666, 0);
    }
    ClearW = sem_open("/ClearW", O_CREAT | O_EXCL, 0666, 0);
    if (ClearW == SEM_FAILED) {
        sem_close(ClearW);
        sem_unlink("/ClearW");
        ClearW = sem_open("/ClearW", O_CREAT, 0666, 0);
    }
}
void DestroySemaphores(void) {
    if (sem_close(CarNum) < 0) {
        perror("Error destroying Semaphore CarNum");
    }
    if (sem_close(CarLimit) < 0) {
        perror("Error destroying Semaphore CarLimit");
    }
    if (sem_close(ClearN) < 0) {
        perror("Error destroying Semaphore NSCars");
    }
    if (sem_close(ClearS) < 0) {
        perror("Error destroying Semaphore NSCars");
    }
    if (sem_close(ClearE) < 0) {
        perror("Error destroying Semaphore NSCars");
    }
    if (sem_close(ClearW) < 0) {
        perror("Error destroying Semaphore NSCars");
    }
    sem_unlink("/CarNum");
    sem_unlink("/CarLimit");
    sem_unlink("/ClearN");
    sem_unlink("/ClearS");
    sem_unlink("/ClearE");
    sem_unlink("/ClearW");
}

void* Intersection(void* argument){
    while (numOfCars != 0) {
    	printf("North/South light has turned green.\n");
        sem_post(ClearN);
        sem_post(ClearS);
        sleep(Light);
        sem_wait(ClearS);
        printf("North/South light has turned yellow.\n");
        sem_wait(ClearN);
        printf("North/South light has turned red.\n");
        printf("East/West light has turned green.\n");
        sem_post(ClearE);
        sem_post(ClearW);
        sleep(Light);
        sem_wait(ClearW);
        printf("East/West light has turned yellow.\n");
        sem_wait(ClearE);
        printf("East/West light has turned red.\n");
    }
    pthread_exit(NULL);
}
void* NorthCar(void* argument){
    int loop = *(int*)argument;
    for (int i = 0; i < loop; ++i) {
        sem_wait(ClearN);
        sem_wait(CarLimit);
        printf("North-bound car %d is moving.\n", i+1);
        fflush(stdout);
        sleep(Car);
        printf("North-bound car %d has moved on.\n", i+1);
        fflush(stdout);
        sem_wait(CarNum);
        numOfCars--;
        sem_post(CarNum);
        sem_post(CarLimit);
        sem_post(ClearN);
        usleep(100);
    }
    pthread_exit(NULL);
}
void* SouthCar(void* argument){
    int loop = *(int*)argument;
    for (int i = 0; i < loop; ++i) {
        sem_wait(ClearS);
        sem_wait(CarLimit);
        printf("South-bound car %d is moving.\n", i+1);
        fflush(stdout);
        sleep(Car);
        printf("South-bound car %d has moved on.\n", i+1);
        fflush(stdout);
        sem_wait(CarNum);
        numOfCars--;
        sem_post(CarNum);
        sem_post(CarLimit);
        sem_post(ClearS);
        usleep(100);
    }
    pthread_exit(NULL);
}
void* EastCar(void* argument){
    int loop = *(int*)argument;
    for (int i = 0; i < loop; ++i) {
        sem_wait(ClearE);
        sem_wait(CarLimit);
        printf("East-bound car %d is moving.\n", i+1);
        fflush(stdout);
        sleep(Car);
        printf("East-bound car %d has moved on.\n", i+1);
        fflush(stdout);
        sem_wait(CarNum);
        numOfCars--;
        sem_post(CarNum);
        sem_post(CarLimit);
        sem_post(ClearE);
        usleep(100);
    }
    pthread_exit(NULL);
}
void* WestCar(void* argument){
    int loop = *(int*)argument;
    for (int i = 0; i < loop; ++i) {
        sem_wait(ClearW);
        sem_wait(CarLimit);
        printf("West-bound car %d is moving.\n", i+1);
        fflush(stdout);
        sleep(Car);
        printf("West-bound car %d has moved on.\n", i+1);
        fflush(stdout);
        sem_wait(CarNum);
        numOfCars--;
        sem_post(CarNum);
        sem_post(CarLimit);
        sem_post(ClearW);
        usleep(100);
    }
    pthread_exit(NULL);
}

int main(int argc, const char * argv[]) {
    char * string = malloc(sizeof(char)*stringlen);
    printf("Please input traffic pattern: ");
    fgets(string, stringlen, stdin);
    printf("\n");
    int N=0,S=0,E=0,W=0;
    for (int i = 0; i < strlen(string); ++i) {
        switch (string[i]) {
            case 'N':
            case 'n':
                ++N;
                break;
            case 'S':
            case 's':
                ++S;
                break;
            case 'E':
            case 'e':
                ++E;
                break;
            case 'W':
            case 'w':
                ++W;
                break;
            default:
                break;
        }
    }
    numOfCars = N + S + E + W;
    printf("%d are approaching the intersection.\n", numOfCars);
    printf("\t -%d are heading north.\n", N);
    printf("\t -%d are heading south.\n", S);
    printf("\t -%d are heading east.\n", E);
    printf("\t -%d are heading west.\n", W);
    CreateSemaphores();
    pthread_t thread0, thread1, thread2, thread3, thread4;
    pthread_create(&thread0, NULL, &Intersection, NULL);
    pthread_create(&thread1, NULL, &NorthCar, &N);
    pthread_create(&thread2, NULL, &SouthCar, &S);
    pthread_create(&thread3, NULL, &EastCar, &E);
    pthread_create(&thread4, NULL, &WestCar, &W);
    
    pthread_join(thread0, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);
    
    printf("All the cars have left.\n");
    DestroySemaphores();
    return 0;
    
}
