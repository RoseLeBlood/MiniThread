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

#include "mn_config_preview.hpp"
#include "mn_autolock.hpp"

#if MN_THREAD_CONFIG_PREVIEW_FUTURE == MN_THREAD_CONFIG_YES

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
    void write(buffer_item_t item) {
		automutx_t lock(m_mLock);

		m_pBuffer[m_iHead] = item;

		if(m_bFull) 
			m_iTail = (m_iTail + 1) % m_szRingBufferSize;

		m_iHead = (m_iHead + 1) % m_szRingBufferSize;
		m_bFull = (m_iHead == m_iTail);
	}
    /**
     * Write items to the ringbuffer
     * @param items The array of items to write to the ring buffer
     * @param offset The write offset
     * @param size The size of the items to writte to the ring buffer
     * 
     * @return The size to written to the ringbuffer, -1 on error
     */ 
    int write(buffer_item_t* items, unsigned int offset, unsigned int size) {
        if (size <= offset) return -1;

        int written = 0;
        for(unsigned int i = offset; i < (size - offset); i++) {
            put(items[i]);
            written++;
        }
        return written;
    }
    /**
     * get a item from the buffer
     * 
     * @return The readed value
     */ 
    buffer_item_t read() {
		automutx_t lock(m_mLock);

		if(is_empty()) {
			return buffer_item_t();
		}

		auto val = m_pBuffer[m_iTail];
		m_bFull = false;
		m_iTail = (m_iTail + 1) % m_szRingBufferSize;

		return val;
	}
    /**
     * get a item from the buffer, don't removed it
     * 
     * @return The readed value
     */ 
    buffer_item_t peek() {
        automutx_t lock(m_mLock);

		if(is_empty()) {
			return buffer_item_t();
		}

		auto val = m_pBuffer[m_iTail];
		m_bFull = false;

		return val;
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
    int read(buffer_item_t* items, unsigned int offset, unsigned int size) {
        if (size <= offset) return -1;

        int readed = 0;
        for(unsigned int i = 0; i < size; i++) {
            items[i + offset] = read();
            readed++;
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
	bool            m_bFull;
};

using circular_buffer_t = basic_circular_buffer;

#endif //MN_THREAD_CONFIG_PREVIEW_FUTURE

#endif // MINLIB_ESP32_RINGBUFFER_