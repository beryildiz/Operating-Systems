#include "queue.hpp"

Queue::Queue () {
    empty = true;
    full = false;
    head = 0;
    tail = 0;
    finished = 0;
    count = 0;
}

Queue::~Queue () { }

void Queue::addItem(string in) {
    buf[tail] = in;
    tail++;
    if (tail == QUEUESIZE)
        tail = 0;
    if (tail == head)
        full = 1;
    empty = 0;
}

string Queue::delItem () {
    count++;
    string out = buf[head];
    head++;
    if (head == QUEUESIZE)
        head = 0;
    if (head == tail)
        empty = 1;
    full = 0;

    return out;
}