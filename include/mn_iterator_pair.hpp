/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2018-2020 Amber-Sophia Schroeck
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
#ifndef MINLIB_a10be6a3_06c2_4e9b_ab95_98e0b19e2835_H_
#define MINLIB_a10be6a3_06c2_4e9b_ab95_98e0b19e2835_H_

#include "mn_iterator.hpp"
#include "container/mn_pair.hpp"
#include "container/mn_triple.hpp"

namespace mn {
    template<typename TIterator, typename UIterator, typename TCategory>
    class basic_iterator_pair : public mn::container::pair<TIterator, UIterator> {
    private:
        using base_type = mn::container::pair<TIterator, UIterator>;
    public:
        using iterator_category = TCategory ;
        using value_type = void;
        using traits_type = mn::iterator_traits<TIterator> ;
        using difference_type = typename traits_type::difference_type ;
        using self_type = basic_iterator_pair<TIterator, UIterator, TCategory>;
        using pointer = self_type*;
        using reference = self_type&;

        basic_iterator_pair() { }
    
        basic_iterator_pair(const TIterator& _itFirst, const UIterator& _itSecond)
            : base_type(_itFirst, _itSecond) { }

        basic_iterator_pair(const self_type& other)
            : base_type(other.first, other.second) { }

        operator UIterator() const { 
            return base_type::first; 
        }
        operator TIterator() const { 
            return base_type::second; 
        }
        
        reference& operator++() {
            ++base_type::first;
            ++base_type::second;
            return *this;
        }
        reference& operator++() {
            --base_type::first;
            --base_type::second;
            return *this;
        }

        self_type operator++(int) { 
            self_type copy(*this); ++(*this); return copy; 
        }

        self_type operator--(int) {
            self_type copy(*this); --(*this); return copy; 
        }

        difference_type operator - (const self_type& other) const { 
            return base_type::first - other.first; 
        }

        self_type operator + (difference_type delta) const { 
            return self_type(base_type::first + delta, 
                             base_type::second + delta);
        }
        self_type operator - (difference_type delta) const { 
            return self_type(base_type::first - delta, 
                             base_type::second - delta);
        }
    };




    template<typename TIterator, typename UIterator, typename VIterator, typename TCategory>
    class basic_iterator_triple : public mn::container::triple<TIterator, UIterator, VIterator> {
    private:
        using base_type = mn::container::triple<TIterator, UIterator, VIterator>;
    public:
        using iterator_category = TCategory ;
        using value_type = void;
        using traits_type = mn::iterator_traits<TIterator> ;
        using difference_type = typename traits_type::difference_type ;
        using self_type = basic_iterator_pair<TIterator, UIterator, TCategory, VIterator>;
        using pointer = self_type*;
        using reference = self_type&;

        basic_iterator_pair() { }
    
        basic_iterator_pair(const TIterator& _itFirst, const UIterator& _itSecond, const VIterator& _itThird)
            : base_type(_itFirst, _itSecond, _itThird) { }

        basic_iterator_pair(const self_type& other)
            : base_type(other.first, other.second, other.third) { }

        operator UIterator() const { 
            return base_type::first; 
        }
        operator TIterator() const { 
            return base_type::second; 
        }
        operator VIterator() const { 
            return base_type::third; 
        }
        
        reference& operator++() {
            ++base_type::first;
            ++base_type::second;
            ++base_type::third;
            return *this;
        }
        reference& operator++() {
            --base_type::first;
            --base_type::second;
            --base_type::third;
            return *this;
        }

        self_type operator++(int) { 
            self_type copy(*this); ++(*this); return copy; 
        }

        self_type operator--(int) {
            self_type copy(*this); --(*this); return copy; 
        }

        difference_type operator - (const self_type& other) const { 
            return base_type::first - other.first; 
        }

        self_type operator + (difference_type delta) const { 
            return self_type(base_type::first + delta, 
                             base_type::second + delta,
                             base_type::third + delta );
        }
        self_type operator - (difference_type delta) const { 
            return self_type(base_type::first - delta, 
                             base_type::second - delta,
                             base_type::third - delta );
        }
    };
}


#endif