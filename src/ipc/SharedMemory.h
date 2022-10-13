// Copyright (C) 2022 All rights reserved.
// Email: oxox0@qq.com. Created in 202207

#ifndef IPC_SHAREDMEMORY_H
#define IPC_SHAREDMEMORY_H

namespace ipc {

/**
 * @brief SharedMemory
 * ipcs -a show the all system information
 * ipcrm remove the system struct
 */
class SharedMemory {
public:
    SharedMemory(): shmid_(0), memory_(nullptr) {}
    // one block size is 4k
    explicit SharedMemory(key, block_size) {
        shmid_ = shmget(key, block_size * 4096, 0666 | IPC_CREAT);
        attach();
    }

    void *memory() const {
        return memory_;
    }

    bool deatch() {
        bool re = shmdt(memory_) != -1;
        if (re) {
            memory_ = nullptr;
        }
        return re;
    }

    void attach() {
        deatch();
        memory_ = shmat(shmid_, 0, 0);
    }

    void attach(int shmid) {
        shmid_ = shmid;
        attach();
    }

    bool del() {
        bool re = shmctl(shmid_, IPC_RMID, 0) != -1;
        if (re)
        {
            shmid_ = 0;
            memory_ = nullptr;
        }
        return re;
    }

private:
    int shmid_;
    void *memory_;

}; // class SharedMemory

} // namespace ipc

#endif // IPC_SHAREDMEMORY_H
