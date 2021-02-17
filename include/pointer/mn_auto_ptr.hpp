/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2018 Amber-Sophia Schroeck
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
#ifndef _MINLIB_AUTO_PTR_H_
#define _MINLIB_AUTO_PTR_H_

namespace mn {
    namespace pointer {
        class basic_auto_ptr_base {
        public:
            void set(const volatile void* p)   { _M_p = const_cast<void*>(p); }
            void set(void* p)                  {  _M_p = p; }
        protected:
            void* _M_p;
        };

        template<class T> 
        class basic_auto_ptr_ref {
        public:
            using value_type = T;
            using self_type = basic_auto_ptr_ref<T>;
            using pointer = T*;
            using ref_type =  basic_auto_ptr_base;
                
            basic_auto_ptr_ref(ref_type& r, pointer p) : m_ref(r), m_pointer(p)  {  }

            pointer release() const {  
                m_ref.set((void*)NULL);  
                return m_pointer; 
            }
            
            self_type& operator = (self_type const&) = delete;
                
            ref_type& m_ref;
            pointer const m_pointer;
        };

        /**
         * Auto pointer template class - from RoseLeBlood/aSTL 
         * 
         * @code c++
         * 
         * #include "include/mn_auto_ptr.hpp"
         * #include <iostream>    
         * 
         * void f(mn::auto_ptr<int> a) {
         *  if(a.get())
         *      std::cout << *(a.get()) << std::endl; 
         *  else
         *      std::cout << "Pointer experide" << std::endl;
         * }
         * mn::auto_ptr<int> ap;
         * int main() {
         * {
         *      int* a = (int*)malloc(4UL); *a = 3;
         *      ap.reset( a );
         *      f(ap);
         *  }
         *  f(ap);
         * }
         * // Ausgabe: 
         * // 3
         * // Pointer experide
         * @endcode
         */ 
        template<class T> 
        class basic_auto_ptr : public basic_auto_ptr_base
        {
        public:
            using value_type = T;
            using pointer = T*;
            using self_type = basic_auto_ptr<value_type> ;

            explicit basic_auto_ptr(value_type* px = 0)  { 
                this->set(px); 
            }
            
            template<class To> 
            basic_auto_ptr(basic_auto_ptr<To>& r)  { 
                pointer conversionCheck = r.release(); 
                this->set(conversionCheck); 
            }
            
            basic_auto_ptr(const self_type& r)  { 
                this->set(r.release()); }
            
            basic_auto_ptr(basic_auto_ptr_ref<value_type> r) { 
                this->set(r.release());  }
            
            ~basic_auto_ptr() { 
                delete this->get(); }
            
            pointer release()  { 
                pointer px = this->get(); 
                this->set((void*)0); 
                return px; 
            }
            void reset(pointer px = 0) { 
                pointer pt = this->get();
                if (px != pt) delete pt;
                    this->set(px);
            }

            pointer operator->() const  {
                if(get() == 0) return 0;
                return this->get();
            }
            value_type& operator*() const  {
                if(get() == 0) return 0;
                return *this->get();
            }
            
            self_type& operator = (basic_auto_ptr_ref<value_type> r)  {
                    reset(r.release());
                    return *this;
            }
            self_type& operator = (self_type& r)  {
                    reset(r.release());
                    return *this;
            }
            self_type& operator = (value_type* px = 0)  {
                    reset(px);
                    return *this;
            }
            
            /**
             * Get the holding object
             * @return The holding object
             */ 
            value_type* get() const {  return static_cast<value_type*>(_M_p); }  

            template<class T1> 
            operator basic_auto_ptr_ref<T1>() {  return basic_auto_ptr_ref<T1>(*this, this->get()); }
            
            template<class T1> 
            operator basic_auto_ptr<T1>() { return basic_auto_ptr<T1>(release()); }
            
        };
    }
}
#endif