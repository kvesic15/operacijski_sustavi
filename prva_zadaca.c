#include<stdio.h>
#include<signal.h>
#include<math.h>
#include<stdlib.h>
#include<unistd.h>

int trenutni_broj=1;

void sigusr1_handler(int sig)
{
printf("Trenutno se obradjuje broj %d.\n", trenutni_broj);
}

void sigterm_handler(int sig)
{
FILE *f=fopen("status", "w");
fprintf(f, "%d\n", trenutni_broj);
fclose(f);
printf("\nProgram prekinut sa SIGTERM signalom. Trenutni broj zapisan u datoteku 'status'.\n");
exit(0);
}

void sigint_handler(int sig)
{
	printf("\nProgram prekinut sa singnalom SIGINT i stanje u datoteci 'status' postaveljno na 0.\n");
	exit(0);
}



int main()
{
signal(SIGUSR1, sigusr1_handler);
signal(SIGTERM, sigterm_handler);
signal(SIGINT, sigint_handler);

int statusni_broj;
int temp;

FILE *f1=fopen("status", "r");
fscanf(f1, "%d", &statusni_broj);
if(statusni_broj==0)
{
	FILE *f2=fopen("obrada", "r");
	while(fscanf(f2, "%d", &temp)==1)
	trenutni_broj=(int)sqrt(temp);
	fclose(f2);
}
if(statusni_broj!=0)
{trenutni_broj=statusni_broj;}
fclose(f1);



FILE *fobrada = fopen("obrada", "a");
FILE *fstatus = fopen("status", "w");

printf("Program pokrenut i zapisuje brojeve u datoteku...\n");


while(1)
{
trenutni_broj++;
if(fstatus != NULL)
    {
    	FILE *fstatus = fopen("status", "w");
        fprintf(fstatus, "0\n");
        fclose(fstatus);
    }

 int kvadrat = trenutni_broj*trenutni_broj;
fprintf(fobrada, "%d\n", kvadrat);
fflush(fobrada);
sleep(8);

}

return 0;
}
