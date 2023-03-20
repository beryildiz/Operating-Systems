#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <thread>
#include <sstream>

#include "web_request.hpp"
#include "queue.hpp"

#define USE_PROXY 0

using std::thread;
using std::string;
using std::stringstream;

class Client{
    private:
        Queue& m_fifo;
        WebRequest& m_req;
        int m_id;
        thread m_thread;

        void clientSpawn();

    public:
        Client(Queue& fifo, WebRequest& req, int id);
        ~Client();

        string getDomain(string url);
        string toString(stringstream& out);

        void client();
        thread* getThread() { return &m_thread; }
};

#endif //QUEUE_HPP