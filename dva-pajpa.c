/*
Koriscenjem programskog jezika Ckreirati Linux program koji kreira dodatni process sa kojim komunicira koriscenjem datavoda. Roditeljski process generise 5slucajnih celih pozitivnih brojeva uopsegu od Odo 99 i salje hi putem datavoda procesu detetu. Proces dete ukoliko je primljeni broj paran koriscenjem datavoda procesu roditelju vraca taj broj. Uslucaju nearnog broja ne radi nista. Na kraju razmene svih 5 slucajno generisanih brojeva process roditelj na ekranu stampa koliko je bilo parnih brojeva.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


int main()
{
    srand(time(NULL));
    int pip1[2];
    int pip2[2];

    if(pipe(pip1)<0 || pipe(pip2)<0 )
        return -1;
    
    if(fork() != 0)
    {
        close(pip1[0]);
        close(pip2[1]);

        for(int i=0;i<5;i++)
        {
            int broj= 88;
            // int broj=rand() % 100;
            write(pip1[1],&broj,sizeof(int));
        }
        close(pip1[1]);
        wait(NULL);
        int p=0;
        int broj1;
        printf("%zd",read(pip2[0],&broj1,sizeof(int)));
        while(read(pip2[0],&broj1,sizeof(int))>0)
            p++;
        
        printf("broj parnih je %d ", p);
        
        close(pip2[0]);

    }
    else
    {
        close(pip1[1]);
        close(pip2[0]);
        int broj2;
        while(read(pip1[0],&broj2,sizeof(int))>0)
        {
            if(broj2%2==0)
            {
                write(pip2[1],&broj2,sizeof(int));
            }
        }

        close(pip1[0]);
        close(pip2[1]);

    }


    return 0;
}