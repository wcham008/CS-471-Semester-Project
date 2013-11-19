#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>
#include <dos.h>

pthread_mutex_t produce_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t consume_mutex = PTHREAD_MUTEX_INITIALIZER;

int             b;    /* buffer size = 1; */

int main(int argc, char *argv[])
{
  srand(time(NULL));

  int N;
  int i;

  if (argc == 2)
  {
    if ( (atoi(argv[1]) > 9) || (atoi(argv[1]) < 1) )
    {
      printf ("\nArgument must be between 1 and 9\n");
      exit(1);
    }
    else
    {
      N = atoi(argv[1]);
    }
  }
  else
  {
    N = 9;
  }
  pthread_t       producer_thread[N];
  pthread_t       consumer_thread[N];
  void           *producer(void *);
  void           *consumer(void *);

  pthread_mutex_lock(&consume_mutex);
  for (i = 0; i < N; i++)
  {
    pthread_create(&producer_thread[i], NULL, producer, (void *)(intptr_t) N);
    printf("Producer Thread %d has been created.\n", i+1);
    pthread_create(&consumer_thread[i], NULL, consumer, (void *)(intptr_t) N);
    printf("Consumer Thread %d has been created.\n", i+1);

    pthread_join(producer_thread[i], NULL);
    pthread_join(consumer_thread[i], NULL);
  }



  printf("DONE\n");
}

void
add_buffer(int i)
{
  b = i;
}
int
get_buffer()
{
  return b;
}

void           *
producer(void *N)
{
  int             i;
  int             r;
  for (i = 0; i < (intptr_t)N; i++) {
    pthread_mutex_lock(&produce_mutex);
    add_buffer(i);
    printf("Producer created item: %d\n", i+1);
    r = rand() % (intptr_t)N + 1;
    Sleep(r);
                fflush(stdout);
    pthread_mutex_unlock(&consume_mutex);
  }
  printf("exit producer\n");
  pthread_exit(NULL);
}
void           *
consumer(void *N)
{
  int     i;
  int     v;
  int     r;
  for (i = 0; i < (intptr_t)N; i++) {
    pthread_mutex_lock(&consume_mutex);
    v = get_buffer();
    printf("Consumer consumed item: %d\n", v+1);
    r = rand() % (intptr_t)N + 1;
    Sleep(r);
                fflush(stdout);
    pthread_mutex_unlock(&produce_mutex);
  }
  printf("exit consumer\n");
  pthread_exit(NULL);
}
