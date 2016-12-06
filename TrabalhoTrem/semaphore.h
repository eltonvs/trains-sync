#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

class Semaphore {
 public:
    Semaphore(key_t, int, int);
    ~Semaphore();
    void P();
    void V();
    int getSemaphoreID();
    int getCounter();

 private:
    union semun{int val;} arg;
    int ID;
    struct sembuf op;
};

#endif // SEMAPHORE_H
