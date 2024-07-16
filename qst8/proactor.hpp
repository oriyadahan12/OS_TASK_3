#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/socket.h>

#include <functional>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std;
typedef std::function<void*(int, mutex&)> proactorFunc;

class proactor {
   private:
    mutex mtx;
    thread server_thread;

   public:
    // starts new proactor and returns proactor thread id.
    void startProactor(int sockfd, proactorFunc threadFunc);
    // stops proactor by threadid
    void stopProactor();
};