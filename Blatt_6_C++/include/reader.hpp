#ifndef READER_HPP
#define READER_HPP

#include <thread>

#include "queue.hpp"

using std::string;
using std::thread;

class Reader{
    private:
        Queue& m_fifo;
        string m_file;
        thread m_thread;

        void readerSpawn();

    public:
        Reader(Queue& fifo, string file);
        ~Reader();

        void reader();
        thread* getThread() { return &m_thread; }
};

#endif //READER_HPP