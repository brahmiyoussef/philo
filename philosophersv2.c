
#include "philosophersv2.h"




int main(int argc,char* argv[]){
// declare the structure 
parameters* arguments=(parameters*)malloc(sizeof(parameters));
//check if structure got allocated successfully
if (arguments == NULL) {
      printf("Error: Memory allocation failed!\n");
      exit(1);
   }


//demand parameters frm user 
//get the number of philosophers 

//get time_to_die
printf("entrez time_to_die :");
scanf("%ld", &arguments->time_to_die);
//printf("number %ld\n", arguments->time_to_die);

//get time_to_eat
printf("entrez time_to_eat :");
scanf("%ld", &arguments->time_to_eat);
//printf("number %ld\n", arguments->time_to_eat);

//get time_to_sleep
printf("entrez time_to_sleep :");
scanf("%ld", &arguments->time_to_sleep);
//printf("number %ld\n", arguments->time_to_sleep);
printf("entrez le nombre des philosophers :");
scanf("%d",&arguments->number_of_philosophers);
//printf("number %d\n", arguments->number_of_philosophers);
if (arguments->number_of_philosophers==1){
    struct timeval chrono3;
    gettimeofday(&chrono3,NULL);
    long long chrono3_in_ms= chrono3.tv_sec*1000000 +chrono3.tv_usec;
    struct timeval chrono2;
    gettimeofday(&chrono2,NULL);
    long long chrono2_in_ms= chrono2.tv_sec*1000000 +chrono2.tv_usec-chrono3_in_ms;
    printf("%lld 1 is thinking\n ",chrono2_in_ms);
    gettimeofday(&chrono2,NULL);
    chrono2_in_ms= chrono2.tv_sec*1000000 +chrono2.tv_usec-chrono3_in_ms;
    printf("%lld 1 has a fork\n  ",chrono2_in_ms);
    usleep(arguments->time_to_die);
    gettimeofday(&chrono2,NULL);
    chrono2_in_ms= chrono2.tv_sec*1000000 +chrono2.tv_usec-chrono3_in_ms;
    printf("%lld 1 died \n ",chrono2_in_ms);
    exit(1);


}
else{
//declare threads and mutexes
pthread_t philosophers[arguments->number_of_philosophers];


//forkmutex
arguments->fork_mtx=(pthread_mutex_t**)malloc((arguments->number_of_philosophers)*sizeof(pthread_mutex_t*));
//check if mutexes got located succesfully
for (int i = 0; i < arguments->number_of_philosophers; i++) {
        arguments->fork_mtx[i] = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
    }


//countermutex
pthread_mutex_t* counter_mutex=malloc(sizeof(pthread_mutex_t));
if (counter_mutex == NULL) {
      printf("Error: Memory allocation failed!\n");
      exit(1);
   }

//index's mutex 
pthread_mutex_t* index_mutex=malloc(sizeof(pthread_mutex_t));
if (index_mutex == NULL) {
      printf("Error: Memory allocation failed!\n");
      exit(1);
   }
arguments->index_mtx=index_mutex;

//pass the value of the pointer to the structure that will be passed to the threads 
arguments->counter_mtx=counter_mutex;
   


//the pointer to the counter variable 
int* counter=malloc(sizeof(int));
if (counter == NULL) {
      printf("Error: Memory allocation failed!\n");
      exit(1);
   }
*counter=0;
arguments->COUNTER=counter;
//printf("le valeur de counter in main  est %i\n",&arguments->COUNTER);
  

//variable index
int* index=malloc(sizeof(int));
if (index == NULL) {
      printf("Error: Memory allocation failed!\n");
      exit(1);
   }
*index=1;
arguments->index=index;
//printf("le valeur de lindex in main  est %i\n",&arguments->index);
 //printf(" %i\n",arguments->index_mtx);


//get number_of_times_each_philosoper_must_eat
printf("entrez number_of_times_each_philosoper_must_eat s'il existe sinon entrez -1 :");
scanf("%ld", &arguments->number_of_times_each_philosoper_must_eat);
//printf("number %i\n", arguments->number_of_times_each_philosoper_must_eat);

//initialise mutexes 
    if (pthread_mutex_init(arguments->counter_mtx, NULL) != 0) {
    printf("Error initializing mutex\n");
    exit(2);}
    if (pthread_mutex_init(arguments->index_mtx, NULL) != 0) {
    printf("Error initializing mutex\n");
    exit(2);}


for (int i; i<arguments->number_of_philosophers;i++){
    if (pthread_mutex_init(arguments->fork_mtx[i], NULL) != 0) {
    printf("Error initializing mutex\n");
    exit(2);}
}

//printf("i don't care\n");
//start chrono
    struct timeval chrono;
    gettimeofday(&chrono,NULL);
    arguments->chrono0_in_ms= chrono.tv_sec*1000000 +chrono.tv_usec;

//create threads
for (int i=0; i<arguments->number_of_philosophers;i++){
    if (pthread_create(&philosophers[i],NULL,&routine,arguments) != 0) {
    printf("Error creating threads\n");
    exit(3);
    }
    
}


//detach threads
  /*for(int i =0;i<arguments->number_of_philosophers;i++){ 
    pthread_detach(philosophers[i]);
    if (pthread_detach(philosophers[i]) != 0) {
        printf("Error detaching thread\n");
        exit(4);
    }}
*/


//joining the threads
  for(int i =0;i<arguments->number_of_philosophers;i++){ 
    if (pthread_join(philosophers[i],NULL) != 0) {
        printf("Error joining thread\n");
        exit(5);
    }}
//printf("i don't care4\n");


//destroy mutexes

for (int i; i<arguments->number_of_philosophers;i++){
    if (pthread_mutex_destroy(arguments->fork_mtx[i]) != 0) {
    printf("Error destroying mutex\n");
    free(arguments->fork_mtx[i]);
    exit(2);}
}
if (pthread_mutex_destroy(arguments->counter_mtx) != 0) {
    printf("Error destroying mutex\n");
    exit(2);}
if (pthread_mutex_destroy(arguments->index_mtx) != 0) {
    printf("Error destroying mutex\n");
    exit(2);}
free(arguments->fork_mtx);
free(counter_mutex);
free(counter);
}

free(arguments);

}
