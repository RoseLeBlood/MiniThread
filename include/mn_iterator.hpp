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
#ifndef MINLIB_77a7a837_8b20_44ce_8a17_e432d8f6f902_H_
#define MINLIB_77a7a837_8b20_44ce_8a17_e432d8f6f902_H_

/// Shorthand for container iteration.
#define for_each(type,i,ctr) for (type i = (ctr).begin(); i != (ctr).end(); ++ i)
/// Shorthand for container reverse iteration.
#define each_for(type,i,ctr) for (type i = (ctr).rbegin(); i != (ctr).rend(); ++ i)


namespace mn {
    struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag: public input_iterator_tag {};
	struct bidirectional_iterator_tag: public forward_iterator_tag {};
	struct random_access_iterator_tag: public bidirectional_iterator_tag {};

    template<typename IterT>
	struct iterator_traits {
	   typedef typename IterT::iterator_category iterator_category;
	};

	template<typename T>          
	struct iterator_traits<T*>  {
	   typedef random_access_iterator_tag iterator_category;
	};

	//-----------------------------------------------------------------------------
	namespace internal {
        template<typename TIter, typename TDist> inline
        void distance(TIter first, TIter last, TDist& dist, std::random_access_iterator_tag) {
            dist = TDist(last - first);
        }

        template<typename TIter, typename TDist> inline
        void distance(TIter first, TIter last, TDist& dist, std::input_iterator_tag) {
            dist = 0;
            while (first != last) {
                    ++dist;
                    ++first;
            }
        }

        template<typename TIter, typename TDist> inline
        void advance(TIter& iter, TDist d, std::random_access_iterator_tag) {
            iter += d;
        }

        template<typename TIter, typename TDist> inline
        void advance(TIter& iter, TDist d, std::bidirectional_iterator_tag) {
            if (d >= 0) {
                while (d--) ++iter;
            } else {
                while (d++)  --iter;
            }
        }
        template<typename TIter, typename TDist> inline
        void advance(TIter& iter, TDist d, std::input_iterator_tag) {
            assert(d >= 0);
            while (d--)  ++iter;
        }
	} 
}

#endif