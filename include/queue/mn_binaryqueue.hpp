#ifndef MINLIB_ESP32_BINQUEUE_
#define MINLIB_ESP32_BINQUEUE_

#include "mn_queue.hpp"

/**
 *  Binary queue with overwrite. This queue can only hold one item.
 *  If sucessive Enqueue operations are called, that item is overwritten
 *  with whatever the last item was.
 */
class basic_binaryqueue : public basic_queue {
public:
    /**
     *  ctor
     * 
     *  @param itemSize Size of an item in a queue.
     */
    explicit basic_binaryqueue(unsigned int itemSize) 
                : basic_queue(1, itemSize) { }

    
    /**
     *  Add an item to the queue.
     *
     *  @param item The item you are adding.
     *  @param timeout not use 
     *  @return 'ERR_QUEUE_OK' when the item in the queue overwrited and 'ERR_QUEUE_NOTCREATED'
     *  the queue not created, please call the function create first
     */
    virtual int enqueue(void *item,  unsigned int timeout = -1);
};

using binaryqueue_t = basic_binaryqueue;

#endif