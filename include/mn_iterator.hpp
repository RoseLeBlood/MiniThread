/**
 * This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
 * Copyright (c) 2021 Amber-Sophia Schroeck
 * 
 * The Mini Thread Library is free software; you can redistribute it and/or modify  
 * it under the terms of the GNU Lesser General Public License as published by  
 * the Free Software Foundation, version 3, or (at your option) any later version.
 * 
 * The Mini Thread Library is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with the Mini Thread  Library; if not, see
 * <https://www.gnu.org/licenses/>.  
 */
#ifndef MINLIB_77a7a837_8b20_44ce_8a17_e432d8f6f902_H_
#define MINLIB_77a7a837_8b20_44ce_8a17_e432d8f6f902_H_

#include "mn_def.hpp"

/// Shorthand for container iteration.
#define for_each(type,i,ctr) for (type i = (ctr).begin(); i != (ctr).end(); ++ i)
/// Shorthand for container reverse iteration.
#define each_for(type,i,ctr) for (type i = (ctr).rbegin(); i != (ctr).rend(); ++ i)


namespace mn {
    struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : public forward_iterator_tag {};
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};

    

    template<typename IterT>
	struct iterator_traits {
	   using iterator_category = typename IterT::iterator_category ;
       using difference_type = typename IterT::difference_type ;
       using value_type = typename IterT::value_type ;
       using pointer = typename IterT::pointer;
       using reference = typename IterT::reference ;
	};

	template<typename T>          
	struct iterator_traits<T*>  {
	   using iterator_category = random_access_iterator_tag;
       using difference_type = ptrdiff_t;
       using value_type = T;
       using pointer = value_type*;
       using reference = value_type& ;
	};

	//-----------------------------------------------------------------------------
	
    namespace internal {
        template<typename TIter> 
        inline typename iterator_traits<TIter>::difference_type 
        distance(TIter first, TIter last, mn::random_access_iterator_tag) {
            return last - first;
        }

        template<typename TIter> 
        inline typename iterator_traits<TIter>::difference_type   
        distance(TIter first, TIter last, mn::input_iterator_tag) {
            typename iterator_traits<TIter>::difference_type _n = 0;

            while (first != last) {
                ++first;
                ++_n;
            }
            return _n;
        }

        template<typename TIter, typename TDistance = typename iterator_traits<TIter>::difference_type> 
        inline void advance(TIter& iter, TDistance d, mn::random_access_iterator_tag) {
            iter += d;
        }
        template<typename TIter, typename TDistance = typename iterator_traits<TIter>::difference_type> 
        inline void advance(TIter& iter, TDistance d, mn::bidirectional_iterator_tag) {
            if (d >= 0) {
                while (d--) ++iter;
            } else {
                while (d++)  --iter;
            }
        }
        template<typename TIter, typename TDistance = typename iterator_traits<TIter>::difference_type> 
        inline void advance(TIter& iter, TDistance d, mn::input_iterator_tag) {
            assert(d >= 0);
            while (d--)  ++iter;
        }
    }

    template<typename TIter>
    inline typename iterator_traits<TIter>::difference_type
    distance(TIter first, TIter last) {
        return internal::distance(first, last, typename iterator_traits<TIter>::iterator_category() );
    }

    template<typename TIter, typename TDistance = typename iterator_traits<TIter>::difference_type> 
    inline void advance(TIter& iter, TDistance d) {
        return internal::advance(iter, d, typename iterator_traits<TIter>::iterator_category() );
    }

    
    template<typename TForwardIter, typename TDistance = typename iterator_traits<TForwardIter>::difference_type>
    TForwardIter next(TForwardIter x, TDistance n = 1) {
        advance<TForwardIter, TDistance> (x, n);
        return x;
    }

    template<typename TBidirectionalIter, typename TDistance = typename iterator_traits<TBidirectionalIter>::difference_type>
    TBidirectionalIter prev(TBidirectionalIter x, TDistance n = 1) {
        advance<TBidirectionalIter, TDistance> (x, -n);
        return x;
    }

}

#endif