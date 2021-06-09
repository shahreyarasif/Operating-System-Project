#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>      //for string functions like strcmp(), strlen(),strc()
#include<errno.h>       //for error detection at run time 
#include<sys/ipc.h>
#include <ctype.h>      //for tolower() function
#include<sys/msg.h>     //for message passing
#include <time.h>
#include <stdbool.h>
#include<unistd.h>      //for sleep() funtion


#define M 333

//function declarations
void           welcomePage(); //this formats console.
struct order   takeOrder();
struct order   orderFormatConverter(char orderNumber[], char name[], char flavor[], char puttings[], char quantity[], int n);
void           sendOrder(struct order Order);
void           lowerCase(char string[]);
void 	       orderMenu();
void 	       progressMenu();
void*          generateRandomOrder(void *p);
void*          generateRandom(void * k);

struct order
{
long int message_type;
char     message[M];
};

int main()
{//main starts

welcomePage();

exit(EXIT_SUCCESS);
}//main ends




void welcomePage()
{

while(true)
{

	system("clear");
	char input[M];
	printf("\n\n\n\n\t\t Welcome to Icecream Factory");
	printf("\n\t1) ORDER THE ICECREAM (MANUAL_USER_INPUT)");
	printf("\n\t2) ORDER THE ICECREAM (RANDOM_COMPUTER_GENERATED_INPUT) 1-20");
	printf("\n\t3) EXIT\n");

	
	scanf("%s", input);
	
	if(strcmp(input, "1")==0)
	{
		orderMenu();
	}
	
	if(strcmp(input,"2")==0)
	{
	
	
		pthread_t thread;
		pthread_create( &thread, NULL, generateRandomOrder, NULL);
		pthread_join(thread, NULL);
		// system("./Client");
	}
	else if(strcmp(input,"3")==0)
	{
		return;
	}	
	else
	{
		printf("\n\n\n\n\t\tPlease write in correct format i.e 1 or 2 or 3");
		sleep(4);
	
	}
}


}

void* generateRandomOrder(void *p)
{


pthread_t orders[20];
int i=0; 
while(i<20)
{
pthread_create(&orders[i], NULL, generateRandom, (void *) &i);
i++;
}
i=0;
while(i<20)
{
pthread_join(orders[i], NULL);
i++;

}

}

void* generateRandom(void * k)
{
struct order order1;
char puttings[M], quantity[M], flavor[M], name[M], orderNumber[M]; 
strcpy(name, "XYZ");

strcpy(flavor, "ABC");
strcpy(orderNumber, k);
strcpy(puttings, "no");
sendOrder( orderFormatConverter(orderNumber, name, flavor, puttings, quantity, 5)) ;

}

void orderMenu()
{

	int repeat;
	char ans[M];
	strcpy(ans,"yes");
	
	while(strcmp(ans,"yes")==0 )
	{
		do
		{
			repeat=0;
			sendOrder(takeOrder());
			printf("\nProgress is shown below\n");
			progressMenu();
			
			printf("\n\n\tDo you want to order again? (yes/no)");
			scanf("%s", ans);
			lowerCase(ans);
			if(strcmp(ans,"no")==0)
			{
			printf("\n\t\tAs you wish sir!");
			struct order p;strcpy( p.message,"end");
			sendOrder(p);
			}
			else if(strcmp(ans,"yes")!=0)
			{
			printf("\n\t\tPlease write in correct format i.e yes or no.");
			repeat=1;
			}
		}
		while(repeat);
	
	}

}


void progressMenu()
{

system("clear");
key_t Id1= 41238;
long int msg_to_recieve = 0;
struct order Order;
int memId = msgget(Id1, 0666 | IPC_CREAT);
	if (memId == -1) 
	{
	  fprintf(stderr,"msgget failed with error : %d  ", errno);
	  exit(EXIT_FAILURE);
	}


while(strcmp(Order.message, "end")!=0)
{ 
if (msgrcv(memId, (void *)&Order, BUFSIZ,  msg_to_recieve, 0) == -1) {
	    perror("msgcrv failed with error");
	    exit(EXIT_FAILURE);
}

printf("%s", Order.message);
}

if (msgctl(memId, IPC_RMID, 0) == -1) 
{
	  perror("msgctl(IPC_RMID) failed");
	  exit(EXIT_FAILURE);
}

}

void sendOrder(struct order Order)
{
key_t Id1= 41239;
int memId = msgget(Id1, 0666 | IPC_CREAT);
	if (memId == -1) 
	{
	fprintf(stderr,"msgget failed with error : %d  ", errno);
	exit(EXIT_FAILURE);
	}


        Order.message_type = 1;
	
        if (msgsnd(memId, (void *)&Order, M, 0) == -1) 
	{
	perror("msgsnd error");
	fprintf(stderr,"msgsnd failed ");
	exit(EXIT_FAILURE);
	}

}



struct order takeOrder()
{

	int repeat=0;
	char puttings[M], quantity[M], flavor[M], name[M], orderNumber[M]; 
	
	system("clear");
	strcpy(orderNumber, "9");
	printf("\n\t ORDER");
	printf("\n\tName Of Customer: ");
	scanf("%s", name);
	printf("\n\tFlavor Of Icecream: ");
	scanf("%s",flavor); 
	printf("\n\tQuantity: ");
	scanf("%s",quantity);
	
	do
	{
		repeat=0;
		printf("\n\tPuttings (yes/no): ");
		scanf("%s",puttings);
		
		lowerCase(puttings);
		
		if(strcmp(puttings,"yes")==0)
		{
			printf("\n\tFlavor Of Puttings: ");
			scanf("%s",puttings);
		}
		else if(strcmp(puttings,"no")==0)
		{
			printf("\tAs you wish sir/miss!\n");
		
		}
		else
		{
			printf("\n\tPlease answer yes or no for puttings!");
			printf("Enter 1 or 0\n");
			//scanf("%d",&repeat);
			repeat=1;
		}
	}
	while(repeat);
	
	
	return orderFormatConverter(orderNumber, name, flavor, puttings, quantity, 5);
}

struct order orderFormatConverter(char orderNumber[], char name[], char flavor[], char puttings[], char quantity[], int n)
{
struct order order1;
int i=0;
char *arrays[]={orderNumber, name, flavor, puttings, quantity};
int k=0;
for( i=0; i< n; i++)
{
int j;
for( j=0; j<strlen(arrays[i]); j++)
order1.message[k++]= arrays[i][j];
order1.message[k++]='~';
}
order1.message[k]='\0';
printf("\n%s\n",order1.message);
return order1; 
}

void lowerCase(char string[])
{
int i=-1;
while (string[++i])
string[i]= tolower(string[i]);
}
