#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<pthread.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<string.h>
#include<unistd.h>      // for sleep()
#include<semaphore.h>   //for process syncronization
#define M 333
int mycounter=0;

 struct order
 {
 long int message_type;
 char     message[M];
 };

 struct orderFormated
 {
 char orderNumber[M];
 char name[M]; 
 char flavor[M];
 char puttings[M];
 char quantity[M];
 };


 void* receiveOrder(void *argument);
 void* distributor(void *argument);
 void * worker1 (void * i);
 void * worker2 (void * i);
 void * worker3 (void * i);
 void * worker4 (void * i);
 void sendProgress(char line[]);
 long long fibo(int num);

 struct orderFormated formatTheOrder(char message[]);
 pthread_mutex_t  lock;
 sem_t            semaphore;
 int runningThreads=0;

  int main()
  {
  sem_init(&semaphore, 0, 1);
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }
  pthread_t orderReceiver;
  pthread_create(&orderReceiver, NULL, (void *) & receiveOrder, NULL); 
  pthread_join(orderReceiver, NULL);
  
  exit(EXIT_SUCCESS);
  }

void* receiveOrder(void *argument)
{

key_t Id1= 41239;
long int msg_to_recieve = 0;
struct order Order;struct orderFormated formatedOrder;

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

if (strcmp(Order.message, "end")!=0)
{
formatedOrder= formatTheOrder(Order.message);

pthread_t distributorT;
pthread_create(&distributorT,NULL, distributor, (void *) &formatedOrder); 


pthread_mutex_lock(&lock);
runningThreads++;
pthread_mutex_unlock(&lock);

}



}


while(runningThreads!=0);


if (msgctl(memId, IPC_RMID, 0) == -1) 
{
	  perror("msgctl(IPC_RMID) failed");
	  exit(EXIT_FAILURE);
}

}




void* distributor(void *argument)
{
struct orderFormated formatedOrder= *( (struct orderFormated *) argument); 
char line[M];

sem_wait(&semaphore);

mycounter++;
printf("\n\nOrder#%d is received by Production Team!\n", mycounter);
// printf("\n\nOrder#%s is received by Production Team!\n", formatedOrder.orderNumber.);
//sendProgress(line);
printf("\n\nDetails of Order are: ");
//sendProgress(line);
printf( "\n\n\tName:     %s",formatedOrder.name);
//sendProgress(line);
printf("\n\n\tFlavor:   %s",formatedOrder.flavor);
//sendProgress(line);
if(strcmp(formatedOrder.puttings, "no")!=0)
{
printf( "\n\n\tPuttings: %s",formatedOrder.puttings);
//sendProgress(line);
}
printf( "\n\n\tQuantity: %s",formatedOrder.quantity);
//sendProgress(line);


pthread_t worker[4];
pthread_create(&worker[0],  NULL, worker1, NULL );
pthread_create(&worker[1],  NULL, worker2, NULL );
pthread_create(&worker[2],  NULL, worker3, NULL );
pthread_create(&worker[3],  NULL, worker4, NULL );
pthread_join  (worker[0], NULL);
pthread_join  (worker[1], NULL);
pthread_join  (worker[2], NULL);
pthread_join  (worker[3], NULL);
sem_post(&semaphore);

pthread_mutex_lock(&lock);
runningThreads--;
pthread_mutex_unlock(&lock);
}


void * worker2 (void * i)
{
printf("\n\tWorker #2 is mixing flavor of icecream. \n");
fibo(20);
printf("\n\tWorker #2 is done with mixing!\n" );
}
void * worker3 (void * i)
{
printf("\n\tWorker #3 is giving finishing touch.\n");
fibo(40);
printf("\n\tWorker #3 is done with giving finishing touch!\n" );
}
void * worker4 (void * i)
{
printf("\n\tWorker #4 is packing icecream.\n");
fibo(30);
printf("\n\tWorker #4 is done with packing icecream!\n");
}

void * worker1 (void * i)
{
printf("\n\tWorker #1 is collecting ingredients for icecream. \n");
fibo(35);
printf("\n\tWorker #1 is done with collecting ingredients for icecream!\n");
}

struct orderFormated formatTheOrder(char message[])
{
struct orderFormated formatedOrder;
char *arrays[]={formatedOrder.orderNumber, formatedOrder.name, formatedOrder.flavor, formatedOrder.puttings, formatedOrder.quantity};

int k=0;
int i =0 ;
for(i=0; i<5; i++)
{
int j;
for( j=0; message[k]!='~'; j++)
arrays[i][j]= message[k++]; 
arrays[i][j]='\0';
k++;
}

return formatedOrder;
}
long long fibo(int num) 
{
    if(num == 0) //Base condition
        return 0;
    else if(num == 1) //Base condition
        return 1;
    else
        return fibo(num-1) + fibo(num-2); //Recursively calls fibo() to find nth fibonacci term.
}
