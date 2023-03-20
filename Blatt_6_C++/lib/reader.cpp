#include <fstream>
#include <iostream>

#include "reader.hpp"
#include "web_request.hpp"

using std::cout;
using std::ifstream;
using std::mutex;
using std::unique_lock;
using std::defer_lock;
using std::stringstream;

Reader::Reader(Queue& fifo, string file): m_fifo(fifo), m_file(file){
    readerSpawn();
}

Reader::~Reader(){}

void Reader::reader(){  
    string url;
    
    unique_lock<mutex> lck(m_fifo.mut, defer_lock);

    ifstream fs(m_file);

    while(fs >> url){
        lck.lock();
        // while -> spinlock
        if (m_fifo.isFull()) {
            cout << "reader: queue FULL\n";
            m_fifo.notFull.wait(lck);
        }
        m_fifo.addItem(url);
        cout << "reader: added " << url << " to queue.\n";
        lck.unlock();
        m_fifo.notEmpty.notify_one();
    }

    if(fs.is_open())
        fs.close();

    m_fifo.setFinished();

    cout << "reader: exited\n";
}

void Reader::readerSpawn() {
    m_thread = thread( [this] { this->reader(); } );
}