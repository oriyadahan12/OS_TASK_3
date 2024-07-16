#include <atomic>
#include <iostream>
#include <unordered_map>
typedef void* (*reactorFunc)(int fd);
class Reactor {
   private:
    std::unordered_map<int, reactorFunc> fdFunctions;
    std::atomic<bool> running;

   public:
    Reactor();
    ~Reactor();
    // adds fd to Reactor (for reading) ; returns 0 on success.
    int addFdToReactor(int fd, reactorFunc func);
    // removes fd from reactor
    int removeFdFromReactor(int fd);
    // stops reactor
    int stopReactor();

    void run();
};