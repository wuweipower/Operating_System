#include<unistd.h>
#include<time.h>
#include<stdio.h>
#include<iostream>
#include<pthread.h>
#include<semaphore.h>
#include<vector>
#include<algorithm>
using namespace std;

int seatNum;
int emptySeat;
int onSeat;
unsigned int times;
sem_t mutex;
sem_t wait;
sem_t barbers;

void getHaircut(){}

struct Cust
{
   int id;
   int arrive;
   Cust(int id, int arrive)
   {
      this->id=id;
      this->arrive=arrive;
   }
};

bool cmp(const Cust& a, const Cust& b)
{
   return a.arrive<b.arrive;
}
void * barber(void*)
{
   while(1)
   {
      sem_wait(&wait);
      sem_wait(&mutex);
      sem_post(&barbers);
      onSeat--;
      emptySeat++;
      sem_post(&mutex);
      sleep(times);
   }
}

void* customer(void* p)
{
   int id = *(int *)p;
   sem_wait(&mutex);
   if(emptySeat>0)
   {
      printf("customer %d: there are %d customers in front waiting for a haircut\n",id,onSeat);
      onSeat++;
      emptySeat--;
      sem_post(&wait);
      sem_post(&mutex);
      sem_wait(&barbers);
      sleep(times);
      printf("customer %d finished haircut\n",id);
   }
   else
   {
      printf("customer %d: no more empty seat chairs, customer leaves\n",id);
      sem_post(&mutex);
   }
}
int main()
{
   int n, barberNum;
   std::cin>>n>>barberNum>>emptySeat>>times;
   vector<Cust> custs;

   onSeat=0;

   for(int i=0;i<n;i++)
   {
      int id,arrive;
      cin>>id>>arrive;
      custs.push_back(Cust(id,arrive));
   }

   sort(custs.begin(),custs.end(),cmp);

   // for(int i=0;i<n;i++)
   // {
   //    cout<<custs[i].id<<custs[i].arrive<<endl;
   // }

   sem_init(&mutex,0,1);
   sem_init(&wait,0,0);
   sem_init(&barbers,0,0);

   pthread_t barber_tid[barberNum],customer_tid[n];

   for(int i =0;i<barberNum;i++)
   {
      pthread_create(&barber_tid[i],NULL,barber,NULL);
      
   }
   for(int i =0,j=0;i<n&&j<=custs[n-1].arrive;j++)
   {
      // sleep(custs[i].arrive);
      // pthread_create(&customer_tid[i],NULL,customer,(void*)(&custs[i].id));
      if(custs[i].arrive==j)
      {
         pthread_create(&customer_tid[i],NULL,customer,(void*)(&custs[i].id));
         i++;
      }
      sleep(1);
   }
   for(int i=0;i<n;i++)
   pthread_join(customer_tid[i],NULL);
   sleep(times);

}

