/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2020 Amber-Sophia Schroeck
*
*The Mini Thread Library is free software; you can redistribute it and/or modify  
*it under the terms of the GNU Lesser General Public License as published by  
*the Free Software Foundation, version 3, or (at your option) any later version.

*The Mini Thread Library is distributed in the hope that it will be useful, but 
*WITHOUT ANY WARRANTY; without even the implied warranty of 
*MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
*General Public License for more details.
*
*You should have received a copy of the GNU Lesser General Public
*License along with the Mini Thread  Library; if not, see
*<https://www.gnu.org/licenses/>.  
*/
#ifndef MINLIB_ESP32_RINGBUFFER_
#define MINLIB_ESP32_RINGBUFFER_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "mn_config.hpp"
#include "mn_autolock.hpp"

/**
 * Template class for a simple circular_buffer
 */ 
template <class T>
class basic_circular_buffer  {
public:
    using buffer_item_t = T;

    /**
     * Construtor set and create the ringbuffer
     */ 
    explicit basic_circular_buffer(unsigned int size) 
        : m_szRingBufferSize(size),
          m_mLock(),
          m_pBuffer(new T[size]),
          m_iHead(0),
          m_iTail(0),
          m_bFull(false) { }

    /**
     * Write a item to the buffer
     * @param item The item to write to the ringbuffer
     */ 
    bool write(buffer_item_t item, TickType_t xTicksToWait) {
        bool written = false;
        TickType_t xTicksEnd = xTaskGetTickCount() + xTicksToWait;
        TickType_t xTicksRemaining = xTicksToWait;

        if(m_mLock->lock(xTicksToWait) == NO_ERROR) {
            m_pBuffer[m_iHead] = item;

            written = (m_pBuffer[m_iHead] == item) ;

            if(m_bFull) 
                m_iTail = (m_iTail + 1) % m_szRingBufferSize;

            m_iHead = (m_iHead + 1) % m_szRingBufferSize;
            m_bFull = (m_iHead == m_iTail);

            
            m_mLock->unlock();
        }
        return written;
	}
    /**
     * Write items to the ringbuffer
     * @param items The array of items to write to the ring buffer
     * @param offset The write offset
     * @param size The size of the items to writte to the ring buffer
     * 
     * @return The size to written to the ringbuffer, -1 on error
     */ 
    int write(buffer_item_t* items, unsigned int offset, unsigned int size, TickType_t xTicksToWait) {
        if (size <= offset) return -1;
        if(items == NULL) return 0;

        TickType_t xTicksEnd = xTaskGetTickCount() + xTicksToWait;
        TickType_t xTicksRemaining = xTicksToWait;

        int written = 0;
        int _size = (size - offset);

        for(int i = offset; (i < _size ) && (xTicksRemaining <= xTicksToWait); i++) {
            if(write(items[i], xTicksRemaining))
                written++;

            if (xTicksToWait != portMAX_DELAY) {
                xTicksRemaining = xTicksEnd - xTaskGetTickCount();
            }
        }
        return written;
    }
    /**
     * get a item from the buffer
     * 
     * @param item [out] The readed value
     * 
     * @return If true then readed from buffer and false when not
     */ 
    bool read(buffer_item_t* item, TickType_t xTicksToWait) {
        bool _ret = false;

        if(m_mLock->lock(xTicksToWait) == NO_ERROR) {  
            if(!is_empty()) {
            
                *item = m_pBuffer[m_iTail];

                m_bFull = false;
                m_iTail = (m_iTail + 1) % m_szRingBufferSize;

                m_mLock->unlock();

                _ret = true;
            }
        }
    
		return _ret;
	}
    /**
     * get a item from the buffer, don't removed it
     * 
     * @param item [out] The readed value
     * @return If true then readed from buffer and false when not
     */ 
    bool peek(buffer_item_t* item, TickType_t xTicksToWait) {
        bool _ret = false;
    
        if(m_mLock->lock(xTicksToWait) == NO_ERROR) {
            if(!is_empty()) {

                *item = m_pBuffer[m_iTail]; 
                _ret = true;
            }
            m_mLock->unlock();
        }

		return _ret;
	}
    /**
     * Read a array of items from the ringbuffer buffer
     * 
     * @param items The pointer af array of the getted items
     * @param offset The read offset
     * @param size The size to read from the buffer
     * 
     * @return The size to readed from the ringbuffer, -1 on error
     */ 
    int read(buffer_item_t* items, unsigned int offset, unsigned int size, TickType_t xTicksToWait) {
        if (size <= offset) return -1;

        TickType_t xTicksEnd = xTaskGetTickCount() + xTicksToWait;
        TickType_t xTicksRemaining = xTicksToWait;

        int readed = 0;
        buffer_item_t item ;

        for(unsigned int i = 0; (i < size) && (xTicksRemaining <= xTicksToWait); i++) {
            if(is_empty()) break; 

            if(read(&item, xTicksRemaining)) {
                items[readed + offset] = item;
                readed++;
            }

            if (xTicksToWait != portMAX_DELAY) {
                xTicksRemaining = xTicksEnd - xTaskGetTickCount();
            }
        }
        return readed;
    }

    /**
     * Reset the ringbuffer.
     */ 
    void reset() {
		automutx_t lock(m_mLock);
		m_iHead = m_iTail;
		m_bFull = false;
	}
    /**
     * Is the buffer empty?
     * @return true If the ringbuffer is empty, false If not
     */ 
    bool is_empty() const               { return (!m_bFull && (m_iHead == m_iTail)); }
    /**
     * Is the buffer full?
     * @return true If the ringbuffer is full, false If not
     */ 
	bool is_full() const                { return m_bFull; }
    /**
     * Get the size of the ringbuffer
     * @return The size of the ringbuffer
     */ 
	size_t get_size() const             { return m_szRingBufferSize; }

    /**
     * Get the count of data in the ring buffer
     * @return The count of data in the ring buffer
     */ 
	size_t get_count() const  {
        if(is_full()) {
			return get_size();
		} else {
            if(m_iHead < m_iTail) {
				return (get_size() + (m_iHead - m_iTail) );
			} else {
                return (m_iHead - m_iTail);
            }
        }
	}

    /**
     * Get the write position (head)
     * @return The write position (head)
     */ 
    const int get_pos_write() const {
        return m_iHead;
    }
    /**
     * Get the read position index  (head)
     * @return The read position  (head)
     */ 
    const int get_pos_read() const {
        return m_iTail;
    }
protected:
    /** A saved / cached copy of the size of the buffer*/
    unsigned int    m_szRingBufferSize;
    /** The lock object for multitasking */
    mutex_t         m_mLock;
    /** The buffer of the ring buffer */
    T*              m_pBuffer;
    /** The head index of the ringbuffer */
    unsigned int    m_iHead;
    /** The tail index of the ringbuffer */
	unsigned int    m_iTail;
    /// Is the buffer full
	bool            m_bFull;
};

template <class T>
using circular_buffer_t = basic_circular_buffer<T>;

#endif // MINLIB_ESP32_RINGBUFFER_