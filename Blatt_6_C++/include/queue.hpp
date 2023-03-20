#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <condition_variable>
#include <mutex>
#include <atomic>
#include <string>

using std::string; 
using std::mutex; 
using std::condition_variable;

#define QUEUESIZE 10

class Queue {
private:
    string buf[QUEUESIZE];
    long head, tail;
    bool full, empty, finished;
    int count;
public:
    mutex mut;
    condition_variable notFull, notEmpty;
public:
    Queue();
    ~Queue();

    void addItem(string in);
    string delItem();
    
    void setFinished(){ finished = true;}
    bool isFull() { return full; }
    bool isEmpty() { return empty; }
    bool isFinished() { return finished; }
    int getCount() { return count; }
};

#endif //QUEUE_HPP