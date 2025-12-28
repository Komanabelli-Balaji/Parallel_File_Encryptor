#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <thread>

#include "ProcessManagement.hpp"
#include "../encryptDecrypt/Cryption.hpp"

ProcessManagement::ProcessManagement() {
    itemsSemaphore = sem_open("/items_semaphore", O_CREAT, 0666, 0);
    emptySlotsSemaphore = sem_open("/empty_slots_semaphore", O_CREAT, 0666, 1000);

    if (itemsSemaphore == SEM_FAILED || emptySlotsSemaphore == SEM_FAILED) {
        perror("sem_open failed");
        exit(EXIT_FAILURE);
    }
    
    shmFd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shmFd == -1) {
        perror("shm_open failed");
        exit(EXIT_FAILURE);
    }

    ftruncate(shmFd, sizeof(SharedMemory));
    sharedMem = static_cast<SharedMemory *> (mmap(nullptr, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0));
    sharedMem->front = 0;
    sharedMem->rear = 0;
    sharedMem->size.store(0);
}

ProcessManagement::~ProcessManagement() {
    munmap(sharedMem, sizeof(SharedMemory));
    shm_unlink(SHM_NAME);

    sem_close(itemsSemaphore);
    sem_close(emptySlotsSemaphore);
    sem_unlink("/items_semaphore");
    sem_unlink("/empty_slots_semaphore");
}

bool ProcessManagement::submitToQueue(std::unique_ptr<Task> task) {
    sem_wait(emptySlotsSemaphore);
    std::unique_lock<std::mutex> lock(queueLock);
    
    if(sharedMem->size.load() >= 1000) {
        std::cout << "Queue is full, cannot submit task." << std::endl;
        return false;
    }

    strcpy(sharedMem->tasks[sharedMem->rear], task->toString().c_str());
    sharedMem->rear = (sharedMem->rear + 1) % 1000;
    sharedMem->size.fetch_add(1);

    lock.unlock();
    sem_post(itemsSemaphore);

    std::thread thread_1(&ProcessManagement::executeTasks, this);
    thread_1.detach();
    return true;
}

void ProcessManagement::executeTasks() {
    sem_wait(itemsSemaphore);
    std::unique_lock<std::mutex> lock(queueLock);

    char taskStr[256];
    strcpy(taskStr, sharedMem->tasks[sharedMem->front]);
    sharedMem->front = (sharedMem->front + 1) % 1000;
    sharedMem->size.fetch_sub(1);

    sem_post(emptySlotsSemaphore);

    std::cout << "Executing child process: " << taskStr << std::endl;
    executeCryption(taskStr);
    lock.unlock();
}
