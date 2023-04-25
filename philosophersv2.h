// import  libraries
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>


typedef struct parameters{
int number_of_philosophers;
long time_to_die;
long time_to_eat;
long time_to_sleep;
int number_of_times_each_philosoper_must_eat;
int* COUNTER;
int* index;
long long chrono0_in_ms;
pthread_mutex_t* index_mtx;
pthread_mutex_t* counter_mtx;
pthread_mutex_t** fork_mtx;
}parameters;


void* routine(void*args){
    char buffer[200];
    //we receave the arguments sent by the 
    parameters* arguments2=(parameters*)(args);
    //we give the thread its index
    //printf("le pointeur de lindex est %i\n",arguments2->index);
    //printf("index mutex pointer value is %i\n",arguments2->index_mtx);
    pthread_mutex_lock(arguments2->index_mtx);
    int philosophers_id=*(arguments2->index);
    //printf("index's value is %i\n",philosophers_id);
    *(arguments2->index)=*(arguments2->index)+1;
    //printf("after being modified %pi\n",*(arguments2->index_mtx));
    pthread_mutex_unlock(arguments2->index_mtx);

    //index is set successfully
    //initiating the chronometer
    struct timeval chrono1;
    gettimeofday(&chrono1,NULL);
    long long chrono1_in_ms= chrono1.tv_sec*1000000 +chrono1.tv_usec;
    //in the beginning of the simulation the philosopher is thinking
    struct timeval current_time;
    long long current_time_in_ms;
    gettimeofday(&current_time,NULL);
    current_time_in_ms =(current_time.tv_sec) * 1000000 + current_time.tv_usec-arguments2->chrono0_in_ms ;
    
    sprintf(buffer,"%lld %d is thinking\n",current_time_in_ms, philosophers_id);
    write(STDOUT_FILENO, buffer, strlen(buffer));
    fflush(stdout);
    int count=0;
    while(count!=arguments2->number_of_times_each_philosoper_must_eat){

        //check if ffirst fork is available
        if(pthread_mutex_trylock(arguments2->fork_mtx[philosophers_id%arguments2->number_of_philosophers])==0 || pthread_mutex_trylock(arguments2->fork_mtx[(philosophers_id+1)%arguments2->number_of_philosophers])==0){
            gettimeofday(&current_time,NULL);
            current_time_in_ms =(current_time.tv_sec) * 1000000 + current_time.tv_usec-arguments2->chrono0_in_ms ;

            sprintf(buffer,"%lld %i has taken a fork \n",current_time_in_ms,philosophers_id);
            write(STDOUT_FILENO, buffer, strlen(buffer));
            fflush(stdout);
            //check if the second fork is available
            if (pthread_mutex_trylock(arguments2->fork_mtx[philosophers_id%arguments2->number_of_philosophers])==0 || pthread_mutex_trylock(arguments2->fork_mtx[(philosophers_id+1)%arguments2->number_of_philosophers])==0){
                gettimeofday(&current_time,NULL);
                current_time_in_ms =(current_time.tv_sec) * 1000000 + current_time.tv_usec-arguments2->chrono0_in_ms ;
                sprintf(buffer,"%lld %i  has taken a fork \n",current_time_in_ms,philosophers_id);
                write(STDOUT_FILENO, buffer, strlen(buffer));
                fflush(stdout);
                gettimeofday(&current_time,NULL);
                current_time_in_ms =(current_time.tv_sec) * 1000000 + current_time.tv_usec-arguments2->chrono0_in_ms ;
                sprintf(buffer,"%lld %i is eating \n",current_time_in_ms,philosophers_id);
                write(STDOUT_FILENO, buffer, strlen(buffer));
                fflush(stdout);
                gettimeofday(&current_time,NULL);
                current_time_in_ms =(current_time.tv_sec) * 1000000 + current_time.tv_usec-chrono1_in_ms ;
                if (current_time_in_ms+ arguments2->time_to_eat>arguments2->time_to_die){
                    usleep(arguments2->time_to_die-current_time_in_ms);
                    gettimeofday(&current_time,NULL);
                    current_time_in_ms =(current_time.tv_sec) * 1000000 + current_time.tv_usec-arguments2->chrono0_in_ms ;
                    sprintf(buffer,"%lld %i died\n",current_time_in_ms,philosophers_id);
                    write(STDOUT_FILENO, buffer, strlen(buffer));
                    fflush(stdout);
                    exit(0);
                }
                else
                { usleep(arguments2->time_to_eat);}
                pthread_mutex_unlock(arguments2->fork_mtx[philosophers_id%arguments2->number_of_philosophers]);
                pthread_mutex_unlock(arguments2->fork_mtx[(philosophers_id+1)%arguments2->number_of_philosophers]);
                
                gettimeofday(&current_time,NULL);
                current_time_in_ms =(current_time.tv_sec) * 1000000 + current_time.tv_usec-arguments2->chrono0_in_ms ;
                sprintf(buffer,"%lld %i is sleeping\n",current_time_in_ms,philosophers_id);
                write(STDOUT_FILENO, buffer, strlen(buffer));
                fflush(stdout);
                gettimeofday(&current_time,NULL);
                current_time_in_ms =(current_time.tv_sec) * 1000000 + current_time.tv_usec-chrono1_in_ms ;
                if (current_time_in_ms+ arguments2->time_to_sleep>arguments2->time_to_die){
                    usleep(arguments2->time_to_die-current_time_in_ms);
                    gettimeofday(&current_time,NULL);
                    current_time_in_ms =(current_time.tv_sec) * 1000000 + current_time.tv_usec-arguments2->chrono0_in_ms ;
                    sprintf(buffer,"%lld %i died\n",current_time_in_ms,philosophers_id);
                    write(STDOUT_FILENO, buffer, strlen(buffer));
                    fflush(stdout);
                    exit(0);
                }
                else
                { usleep(arguments2->time_to_sleep);}
                gettimeofday(&current_time,NULL);
                current_time_in_ms =(current_time.tv_sec) * 1000000 + current_time.tv_usec-arguments2->chrono0_in_ms ;
                sprintf(buffer,"%lld %d is thinking\n",current_time_in_ms, philosophers_id);
                write(STDOUT_FILENO, buffer, strlen(buffer));
                fflush(stdout);
                gettimeofday(&chrono1,NULL);
                chrono1_in_ms= chrono1.tv_sec*1000000 +chrono1.tv_usec;
                count++;
            }
            else{
                gettimeofday(&current_time,NULL);
                current_time_in_ms =(current_time.tv_sec) * 1000000 + current_time.tv_usec-chrono1_in_ms ;
                if (current_time_in_ms>arguments2->time_to_die){
                    sprintf(buffer,"%lld %i died\n",current_time_in_ms,philosophers_id);
                    write(STDOUT_FILENO, buffer, strlen(buffer));
                    fflush(stdout);
                    exit(0);
                }

            }
        }
        
        else{
            gettimeofday(&current_time,NULL);
            current_time_in_ms =(current_time.tv_sec) * 1000000 + current_time.tv_usec-chrono1_in_ms ;
            if (current_time_in_ms>arguments2->time_to_die){
            sprintf(buffer,"%lld %i died\n",current_time_in_ms,philosophers_id);
            write(STDOUT_FILENO, buffer, strlen(buffer));
            fflush(stdout);
            exit(0);
            }
            }
    }
    gettimeofday(&current_time,NULL);
    current_time_in_ms =(current_time.tv_sec) * 1000000 + current_time.tv_usec-chrono1_in_ms ;
    sprintf(buffer,"%lld %i have eaten %i time\n",current_time_in_ms,philosophers_id, arguments2->number_of_times_each_philosoper_must_eat);
    write(STDOUT_FILENO, buffer, strlen(buffer));
    return(0);

}
