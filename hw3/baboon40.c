#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#define EAST 0
#define WEST 1
#define ARRIVAL 0
#define ONROPE 1
#define CROSSED 2
#define NUMBABOON 10

typedef struct baboon{
  int dir; // 0-- east; 1 -- west
  int id;
  int status;
}Baboon;

int* eastBaboon; 
int* westBaboon;
int* twoSideBaboons;

void arrive();
void onRope();
void cross();
void fly_Baboons_Fly();
void displayBaboon(Baboon* aBaboon);
void* create_shared_memory(size_t size);

int main(int argc, char** argv){
  
  sem_unlink("directionLockEAST");
    sem_unlink("directionLockWEST");
    sem_unlink("ropeLock");

    twoSideBaboons = (int*)create_shared_memory(8);
    twoSideBaboons[WEST] = 0;
    twoSideBaboons[EAST] = 0;

    pid_t pid;
    int i;

    for(i = 0; i < NUMBABOON; i++) {
      pid = fork();
      if (pid <0) {
          perror("Unable to create a new process\n");
          exit(1);
      }
      else if (pid == 0) {
        break;
      }
  }

  if (pid != 0) {
      while(pid = waitpid(-1, NULL,0)) {
          if(errno == ECHILD) { // inspired by mark
        break;  
          }
      }
      printf("\nMission Complete!\n");
      sem_unlink("ropeLock");
      sem_unlink("directionLockEAST");
      sem_unlink("directionLockWEST");
  }
    else {

      Baboon* aBaboon = malloc(sizeof(Baboon));
      aBaboon->id = i;
      fly_Baboons_Fly(aBaboon);
      free(aBaboon);
      exit(0);
    }
}

void arrive() {
  int toSleep = rand() % 6 + 1;  
    sleep(toSleep);
}

void onRope() {
  sleep(1);
}

void cross() {
  sleep(4);
}

void fly_Baboons_Fly(Baboon* aBaboon) {
    sem_t* directionLock[2];
    directionLock[EAST] = sem_open("directionLockEAST", O_CREAT, S_IRUSR | S_IWUSR, 1);
    directionLock[WEST] = sem_open("directionLockWEST", O_CREAT, S_IRUSR | S_IWUSR, 1);  

    sem_t* ropeLock;
    ropeLock = sem_open("ropeLock", O_CREAT, S_IRUSR | S_IWUSR, 1);

    sem_t* starvation;
    starvation = sem_open("starvation", O_CREAT, S_IRUSR | S_IWUSR, 1);

    srand((time(NULL)%getpid())<<5);
    int direction = rand() % 2; 
    
    arrive();
    aBaboon->dir = direction;
    aBaboon->status = ARRIVAL;
    displayBaboon(aBaboon);
    sem_wait(starvation);
    sem_wait(directionLock[direction]);
    /*if(direction) {
      *westBaboon = *westBaboon + 1;
    }
    else {
      *eastBaboon = *eastBaboon + 1;
    }*/
    twoSideBaboons[direction]++;
    if(twoSideBaboons[direction] == 1) {
      sem_wait(ropeLock);
    } 
    
    onRope();
    aBaboon->status = ONROPE;
    displayBaboon(aBaboon);
    sem_post(directionLock[direction]);

    cross();
    aBaboon->status = CROSSED;
    displayBaboon(aBaboon);

    sem_post(starvation);
    sem_wait(directionLock[direction]);
    twoSideBaboons[direction]--;
    if(twoSideBaboons[direction] == 0) {
      sem_post(ropeLock);
    }
    sem_post(directionLock[direction]);
}

// Cited from stackoverflow 
//https://stackoverflow.com/questions/5656530/how-to-use-shared-memory-with-linux-in-c
void* create_shared_memory(size_t size) {
  // Our memory buffer will be readable and writable:
  int protection = PROT_READ | PROT_WRITE;

  // The buffer will be shared (meaning other processes can access it), but
  // anonymous (meaning third-party processes cannot obtain an address for it),
  // so only this process and its children will be able to use it:
  int visibility = MAP_ANONYMOUS | MAP_SHARED;

  // The remaining parameters to `mmap()` are not important for this use case,
  // but the manpage for `mmap` explains their purpose.
  return mmap(NULL, size, protection, visibility, 0, 0);
}

void displayBaboon(Baboon* aBaboon) {
  if(aBaboon->dir) {
    if(aBaboon->status == 0) {
      printf("Baboon %d arrives at the rope and will go Westward\n", aBaboon->id);
    }
    else if(aBaboon->status == 1) {
      printf("Baboon %d is on the rope and will go Westward\n", aBaboon->id);
    }
    else {
      printf("Baboon %d crossed rope and is in the West side\n", aBaboon->id);
    }
  }
  else {
    if(aBaboon->status == 0) {
      printf("Baboon %d arrives at the rope and will go Eastward\n", aBaboon->id);
    }
    else if(aBaboon->status == 1) {
      printf("Baboon %d is on the rope and will go Eastward\n", aBaboon->id);
    }
    else {
      printf("Baboon %d crossed rope and is in the East side\n", aBaboon->id);
    }
  }
}














