/*
Korišćenjem programskog jezika C kreirati Linux program koji kreira dve dodatne niti 
koje komuniciraju korišćenjem deljenog niza od 5 celobrojnih vrednosti. 
Glavna nit u deljeni niz upisuje slučajno generisane vrednosti u opsegu -10 do 10. 
Po završetku popunjavanja niza prva dodatna nit u tom deljenom nizu sve negativne vrednosti menja 
njihovim apsolutnim vrednostima. Kada prva dodatna nit završi modifikaciju niza 
druga dodatna nit prvi element niza menja srednjom vrednošću svih elemenata niza. 
Pošto druga dodatna nit završi upis vrednosti na poziciju prvog elementa niza glavni program na ekranu 
štampa sve elemente niza i izlazi.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int niz[5];
pthread_mutex_t mutex;
pthread_cond_t nit1,nit2,glavna;
int prva=0,druga=0;

void* f1(void *arg)
{

    pthread_mutex_lock(&mutex);
    while(!prva)
    {
        pthread_cond_wait(&nit1,&mutex);
    }
    for(int i=0;i<5;i++)
    {
        if(niz[i]<0)
        {
            niz[i] = -niz[i];
        }
    }
    prva=0;
    druga=1;
    pthread_cond_signal(&nit2);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void* f2(void*arg)
{

    pthread_mutex_lock(&mutex);
    while(!druga)
        pthread_cond_wait(&nit2,&mutex);
    
    int sum=0;
    for(int i=0;i<5;i++)
    {
        sum+=niz[i];
    }
    niz[0]=sum/5;

    druga=0;
    pthread_cond_signal(&glavna);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main()
{

    pthread_t n1,n2;
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&nit1,NULL);
    pthread_cond_init(&nit2,NULL);
    pthread_cond_init(&glavna,NULL);

    pthread_create(&n1,NULL,f1,NULL);
    pthread_create(&n2,NULL,f2,NULL);

    pthread_mutex_lock(&mutex);
    for(int i=0;i<5;i++)
    {
        niz[i] = rand() % 21 - 10;
    }
    printf("Pocetni niz:\n");
    for(int i=0;i<5;i++)
    {
        printf("%d ",niz[i]);
    }
    prva = 1;
    pthread_cond_signal(&nit1);
    pthread_mutex_unlock(&mutex);

    pthread_mutex_lock(&mutex);
    while(prva || druga){
        pthread_cond_wait(&glavna,&mutex);
    }
    printf("\nNiz posle obrade:\n");
    for(int i=0;i<5;i++)
    {
        printf("%d ",niz[i]);
    }
    pthread_mutex_unlock(&mutex);
    pthread_cond_destroy(&nit1);
    pthread_cond_destroy(&nit2);
    pthread_cond_destroy(&glavna);
    pthread_mutex_destroy(&mutex);

    return 0;
}