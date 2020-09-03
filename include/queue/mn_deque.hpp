#ifndef MINLIB_ESP32_DEQUE_
#define MINLIB_ESP32_DEQUE_

#include "mn_queue.hpp"

/**
 *  A deque class that implements a double ended queue.
 */
class basic_deque : public basic_queue {
public:
    /**
     *  ctor
     * 
     *  @param itemSize Size of an item in a queue.
     */
    basic_deque(unsigned int maxItems, unsigned int itemSize) 
                : basic_queue(maxItems, itemSize) { }

    
    /**
     *  Add an item to the front of the queue. This will result in
     *  the item being removed first, ahead of all of the items
     *  added by the base cal dequeue() function.
     *
     *  @param item The item you are adding.
     *  @param Timeout How long to wait to add the item to the queue if
     *         the queue is currently full.
     *  @return 'ERR_QUEUE_OK' the item was added, 'ERR_QUEUE_ADD' on an error
     *          and 'ERR_QUEUE_NOTCREATED' when the queue not created
     */
    virtual int enqueue_front(void* item, unsigned int timeout = (unsigned int) 0xffffffffUL);
};

using deque_t = basic_deque;

#endif