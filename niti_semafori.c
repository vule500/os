/*
Koriscenjem programskog jezika C kreirati Linux program koji kao argumente komandne linije privata imena dve tekstualne datoteke. Program kreira dve niti. Prva nit otvara datoteku dije je ime prui argument komandne linije i dita liniju po liniju. Druga nit na isti nacin otvara i dita liniju po liniju iz datoteke je je ime drugi argument komandne linije. Ove dive niti naizmenieno upisuju procitane linije u zajednicku izlaznu datoteku pod imenom zbir. txt tako da su neparne linije
u birnoj datoteci iz pre ulazne datoteke, a parne linije iz druge ulazne datoteke. Svaka nit ispred linije teksta u zbirnu datoteku treba da upise ime izvorisne datoteke i broj linije iz izvorisne datoteke
sa koje je procitana ta linije teksta.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

sem_t s1,s2;



void* f1(void* arg)
{
    FILE* f1;
    char* ime=(char*)arg;
    f1=fopen(ime,"r");
    int k=1;
    while(!feof(f1))
    {

        char linija[80];
        fgets(linija,80,f1);

        sem_wait(&s2);
        FILE *f = fopen("zbir.txt","a");
        fprintf(f,"%s %d %s",ime,k,linija);
        k++;
        fclose(f);
        sem_post(&s1);

    }

    fclose(f1);

    return NULL;
}
void* f2(void* arg)
{
    FILE* f2;
    char* ime=(char*)arg;
    f2=fopen(ime,"r");
    int k=1;
    while(!feof(f2))
    {

        char linija[80];
        fgets(linija,80,f2);

        sem_wait(&s1);
        FILE *f = fopen("zbir.txt","a");
        fprintf(f,"%s %d %s",ime,k,linija);
        k++;
        fclose(f);
        sem_post(&s2);

    }

    fclose(f2);

    return NULL;
}


int main(int argc,char* argv[])
{

    pthread_t n1,n2;
    sem_init(&s1,0,0);
    sem_init(&s2,0,1);

    pthread_create(&n1,NULL,f1,(void*)argv[1]);
    pthread_create(&n2,NULL,f2,(void*)argv[2]);

    pthread_join(n1,NULL);
    pthread_join(n2,NULL);


    sem_destroy(&s1);   
    sem_destroy(&s2);   
    return 0;
}