#include "semaphore.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Semaphore Constructor
 *
 * @param key Key to semaphore creation
 * @param iniValue Initial value to semaphore counter
 * @param flags Permission Flags
 */
Semaphore::Semaphore(key_t key, int iniValue, int flags) {
    // Allocate just 1 semaphore
    ID = semget(key, 1, flags);

    // Verify errors
    if (ID == -1) {
        perror("Error ao criar semaforo\n");
        exit(1);
    }

    // Change settings struct value
    arg.val = iniValue;

    // Update Semaphore counter through SETVAL command
    semctl(ID, 0, SETVAL, arg);
}

/**
 * Semaphore Destructor (delete him)
 */
Semaphore::~Semaphore() {
    semctl(ID, 0, IPC_RMID, arg);
}

/**
 * Function to decrement semaphore ("proberen")
 * Used to enter in Critical Sections
 */
void Semaphore::P() {
    //o grupo é formado por apenas 1 semaforo
    op->sem_num = 0;
    //P()
    op->sem_op = -1;
    // Ao finalizar o processo, outros processos que estao pendentes pelo semaforo sao desbloqueados
    op->sem_flg = SEM_UNDO;

    semop(ID, op, 1);
}

/**
 * Function to increment semaphore ("verhogen")
 * Used to exit from Critical Sections
 */
void Semaphore::V() {
    op->sem_num = 0; //o grupo é formado por apenas 1 semaforo
    op->sem_op = 1; //P()
    // Ao finalizar o processo, outros processos que estao pendentes pelo semaforo sao desbloqueados
    op->sem_flg = SEM_UNDO;
    semop(ID, op, 1);
}

/**
 * Gets the semaphore ID
 * @return The semaphore ID
 */
int Semaphore::getSemaphoreID() {
    return ID;
}

/**
 * Gets the semaphore counter
 * @return The semaphore counter
 */
int Semaphore::getCounter() {
    return semctl(ID, 0, GETVAL, arg);
}
