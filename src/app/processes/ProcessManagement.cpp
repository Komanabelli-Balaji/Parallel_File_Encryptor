#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>
#include <atomic>
#include <sys/fcntl.h>
#include <sys/mman.h>

#include "ProcessManagement.hpp"
#include "../encryptDecrypt/Cryption.hpp"

ProcessManagement::ProcessManagement() {
    itemsSemaphore = sem_open("/items_semaphore", O_CREAT, 0666, 0);
    emptySlotsSemaphore = sem_open("/empty_slots_semaphore", O_CREAT, 0666, 1000);
    
    shmFd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shmFd, sizeof(SharedMemory));
    sharedMem = static_cast<SharedMemory *> (mmap(nullptr, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0));
    sharedMem->front = 0;
    sharedMem->rear = 0;
    sharedMem->size.store(0);
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

    int pid = fork(); // pid === process id
    if(pid < 0) {
        return false;
    } else if(pid > 0) {
        std::cout << "Entering the parent process" << std::endl;
        // waitpid();
    } else {
        std::cout << "Entering the child process" << std::endl;
        executeTasks();
        std::cout << "Exiting the child process" << std::endl;
        exit(0);
    }
    return true;
}

void ProcessManagement::executeTasks() {
    sem_wait(itemsSemaphore);
    std::unique_lock<std::mutex> lock(queueLock);

    char taskStr[256];
    strcpy(taskStr, sharedMem->tasks[sharedMem->front]);
    sharedMem->front = (sharedMem->front + 1) % 1000;
    sharedMem->size.fetch_sub(1);

    lock.unlock();
    sem_post(emptySlotsSemaphore);

    std::cout << "Executing child process: " << taskStr << std::endl;
    executeCryption(taskStr);
}

ProcessManagement::~ProcessManagement() {
    munmap(sharedMem, sizeof(SharedMemory));
    shm_unlink(SHM_NAME);
}










// int childProcessToRun = fork();
        // if (childProcessToRun == 0) {
        //     // Child process
        //     std::string taskStr = taskToExecute->toString();
        //     char* args[3];
        //     args[0] = strdup("./cryption");  // Use the correct path to your cryption executable
        //     args[1] = strdup(taskStr.c_str());
        //     args[2] = nullptr;
        //     execv("./cryption", args);  // Use the correct path to your cryption executable
        //     // If execv returns, there was an error
        //     std::cerr << "Error executing cryption" << std::endl;
        //     exit(1);
        // } else if (childProcessToRun > 0) {
        //     // Parent process
        //     // Wait for the child process to complete
        //     int status;
        //     waitpid(childProcessToRun, &status, 0);
        // } else {
        //     // Fork failed
        //     std::cerr << "Fork failed" << std::endl;
        //     exit(1);
        // }