/*
Korišćenjem programskog jezika C kreirati dva Linux procesa koji generišu slučajne celobrojne vrednosti i 
upisuju ih u datoteku brojevi.dat. Prvi proces generiše niz od tri pozitivne celobrojne vrednosti i upisuje 
ih u datoteku. Nakon toga drugi proces generiše jednu negativnu celobrojnu vrednost i upisuje je u datoteku. 
Postupak se ponavlja 20 puta (naizmenično prvi pa drugi proces).
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>


#define MUTEX_KEY 10001
#define SEM1_KEY 10002
#define SEM2_KEY 10003

union semunn {
    int val;
    struct semid_ds *buf;
    ushort *array;
    
};

int main(int argc, char* argv[]) {

    int mutex_id, sem1_id, sem2_id;
    struct sembuf lock = { 0, -1, 0};
    struct sembuf unlock = { 0, 1, 0};
    union semunn semopts;

    mutex_id = semget(MUTEX_KEY, 1, 0666 | IPC_CREAT);
    sem1_id = semget(SEM1_KEY, 1, 0666 | IPC_CREAT);
    sem2_id = semget(SEM2_KEY, 1, 0666 | IPC_CREAT);

    semopts.val = 1;
    semctl(mutex_id, 0, SETVAL, semopts);
    semopts.val = 1;
    semctl(sem1_id, 0, SETVAL, semopts);
    semopts.val = 0;
    semctl(sem2_id, 0, SETVAL, semopts);


    FILE* fajl;

    if(fork() != 0){
        mutex_id = semget((key_t)MUTEX_KEY, 1, 0666);
        sem1_id = semget((key_t)SEM1_KEY, 1, 0666);
        sem2_id = semget((key_t)SEM2_KEY, 1, 0666);

        for(int i=0;i<5;i++)
        {
            semop(sem1_id,&lock,1);
            semop(mutex_id,&lock,1);
            fajl = fopen("brojevi.dat","a");
            int broj;
            for(int i=0;i<3;i++){
                broj = rand() % 100;
                fprintf(fajl,"roditelj upisuje: %d\n",broj);
            }
            fclose(fajl);
            semop(mutex_id,&unlock,1);
            semop(sem2_id,&unlock,1);
        }
        wait(NULL);
        semctl(mutex_id, 0, IPC_RMID, 0);
        semctl(sem1_id, 0, IPC_RMID, 0);
        semctl(sem2_id, 0, IPC_RMID, 0);
    }
    else
    {
        mutex_id = semget((key_t)MUTEX_KEY, 1, 0666);
        sem1_id = semget((key_t)SEM1_KEY, 1, 0666);
        sem2_id = semget((key_t)SEM2_KEY, 1, 0666);
        for(int i=0;i<5;i++)
        {
            semop(sem2_id,&lock,1);
            semop(mutex_id,&lock,1);
            fajl = fopen("brojevi.dat","a");
            int broj = rand() % 100 - 100;
            fprintf(fajl,"dete upisuje: %d\n",broj);
            fclose(fajl);
            semop(mutex_id,&unlock,1);
            semop(sem1_id,&unlock,1);
        }
        wait(NULL);
        semctl(mutex_id, 0, IPC_RMID, 0);
        semctl(sem1_id, 0, IPC_RMID, 0);
        semctl(sem2_id, 0, IPC_RMID, 0);
    }



    return 0;
}