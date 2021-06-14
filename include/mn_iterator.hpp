/**
 * @file
 * This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
 * @author Copyright (c) 2021 Amber-Sophia Schroeck
 * @par License
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

#include "mn_config.hpp"
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
        constexpr typename iterator_traits<TIter>::difference_type
        distance(TIter first, TIter last, mn::random_access_iterator_tag) {
            return last - first;
        }

        template<typename TIter>
        constexpr typename iterator_traits<TIter>::difference_type
        distance(TIter first, TIter last, mn::input_iterator_tag) {
            typename iterator_traits<TIter>::difference_type _n = 0;

            while (first != last) {
                ++first;
                ++_n;
            }
            return _n;
        }

        template<typename TIter, typename TDistance = typename iterator_traits<TIter>::difference_type>
        constexpr void advance(TIter& iter, TDistance d, mn::random_access_iterator_tag) {
            iter += d;
        }
        template<typename TIter, typename TDistance = typename iterator_traits<TIter>::difference_type>
        constexpr void advance(TIter& iter, TDistance d, mn::bidirectional_iterator_tag) {
            if (d >= 0) {
                while (d--) ++iter;
            } else {
                while (d++)  --iter;
            }
        }
        template<typename TIter, typename TDistance = typename iterator_traits<TIter>::difference_type>
        constexpr void advance(TIter& iter, TDistance d, mn::input_iterator_tag) {
            assert(d >= 0);
            while (d--)  ++iter;
        }
    }

    template<typename TIter>
    constexpr typename iterator_traits<TIter>::difference_type
    distance(TIter first, TIter last) {
        return internal::distance(first, last, typename iterator_traits<TIter>::iterator_category() );
    }

    template<typename TIter, typename TDistance = typename iterator_traits<TIter>::difference_type>
    constexpr void advance(TIter& iter, TDistance d) {
        return internal::advance(iter, d, typename iterator_traits<TIter>::iterator_category() );
    }


    template<typename TForwardIter,
			 typename TDistance = typename iterator_traits<TForwardIter>::difference_type>
    TForwardIter next(TForwardIter x, TDistance n = 1) {
        advance<TForwardIter, TDistance> (x, n);
        return x;
    }

    template<typename TBidirectionalIter,
    		 typename TDistance = typename iterator_traits<TBidirectionalIter>::difference_type>
    TBidirectionalIter prev(TBidirectionalIter x, TDistance n = 1) {
        advance<TBidirectionalIter, TDistance> (x, -n);
        return x;
    }

    template <typename TIterator>
	class reverse_iterator {
	public:
		using value_type = typename iterator_traits<TIterator>::value_type;

		using difference_type = typename iterator_traits<TIterator>::difference_type;
		using pointer = typename iterator_traits<TIterator>::pointer;
		using reference = typename iterator_traits<TIterator>::reference;
		using iterator_category = typename iterator_traits<TIterator>::iterator_category;
		using iterator = TIterator;

		using self_type = reverse_iterator<TIterator>;

		constexpr reverse_iterator ()
			: m_itter() {}
		constexpr explicit	reverse_iterator (TIterator iter)
			: m_itter (iter) {}

		constexpr iterator base () const {
			return m_itter;
		}

		constexpr reference	operator* () const {
			iterator prev (m_itter); --prev;
			return *prev;
		}
		constexpr pointer			operator-> () const {
			return &(operator*()); }

		constexpr self_type&	operator++ () {
			-- m_itter; return *this; }

		constexpr self_type&	operator-- () {
			 ++ m_itter; return *this; }

		constexpr self_type	operator++ (int) {
			self_type prev (*this); -- m_itter; return prev; }

		constexpr self_type	operator-- (int) {
			self_type prev (*this); ++ m_itter; return prev; }

		constexpr self_type&	operator+= (size_t n) {
			m_itter -= n; return *this; }

		constexpr self_type&	operator-= (size_t n) {
			m_itter += n; return *this; }

		constexpr self_type	operator+ (size_t n) const {
			return self_type (m_itter - n); }

		constexpr self_type	operator- (size_t n) const {
			return self_type (m_itter + n); }

		constexpr bool operator== (const self_type& iter) const {
			return m_itter == iter.m_itter; }

		constexpr bool operator< (const self_type& iter) const {
			return iter.m_itter < m_itter; }

		constexpr reference			operator[] (size_t n) const {
			return *(*this + n); }

		constexpr difference_type	operator -  (const self_type& i) const {
			return mn::distance (i.m_itter, m_itter); }
	protected:
		iterator m_itter;
	};

	template <typename TCon>
	class insert_iterator {
	public:
		using container_type = TCon;
		using iterator_category = output_iterator_tag;

		using value_type = typename container_type::value_type;
		using difference_type = typename container_type::difference_type	;
		using pointer = typename container_type::pointer;
		using reference = typename container_type::reference;
		using const_reference = typename container_type::const_reference;
		using iterator = typename container_type::iterator;

		constexpr explicit insert_iterator (container_type& ctr, iterator itr)
			: m_con (ctr), m_iter (m_iter) {}

		inline insert_iterator&	operator= (const_reference  v) {
			m_iter = m_con.insert (m_iter, v); return *this;
		}

		constexpr insert_iterator&	operator* (void)  {
			return *this;
		}

		constexpr insert_iterator&	operator++ (void) {
			++ m_iter; return *this;
		}

		constexpr insert_iterator	operator++ (int)  {
			insert_iterator prev (*this);
			++m_iter; return prev;
		}
	protected:
		container_type&			m_con;
		iterator			m_iter;
	};



	template <typename TCon>
	class back_insert_iterator {
	public:
		using container_type = TCon;
		using iterator_category = output_iterator_tag;

		using value_type = typename container_type::value_type;
		using difference_type = typename container_type::difference_type	;
		using pointer = typename container_type::pointer;
		using reference = typename container_type::reference;
		using const_reference = typename container_type::const_reference;
		using iterator = typename container_type::iterator;

		constexpr explicit back_insert_iterator (container_type& ctr)
			: m_con (ctr) {}

		inline back_insert_iterator& operator = (const_reference v) {
			m_con.push_back (v); return *this;
		}

		constexpr back_insert_iterator&	operator*  ()   { return *this; }
		constexpr back_insert_iterator&	operator++ ()   { return *this; }
		constexpr back_insert_iterator	operator++ (int){ return *this; }
	protected:
		container_type&		m_con;
	};

	template <class TCon>
	class front_insert_iterator {
	public:
		using container_type = TCon;
		using iterator_category = output_iterator_tag;

		using value_type = typename container_type::value_type;
		using difference_type = typename container_type::difference_type	;
		using pointer = typename container_type::pointer;
		using reference = typename container_type::reference;
		using const_reference = typename container_type::const_reference;
		using iterator = typename container_type::iterator;

		constexpr explicit	front_insert_iterator (container_type& ctr)
			: m_con (ctr) {}

		inline front_insert_iterator& operator= (const_reference v) {
			m_con.push_front (v);
			return *this;
		}

		constexpr front_insert_iterator&	operator*  ()    { return *this; }
		constexpr front_insert_iterator&	operator++ ()    { return *this; }
		constexpr front_insert_iterator 	operator++ (int) { return *this; }
	protected:
		container_type&		m_con;
	};

	template <typename TIterator>
	class move_iterator {
	public:
		using self_type = move_iterator<TIterator>;

		using iterator = TIterator;

		using value_type = typename iterator_traits<TIterator>::value_type;
		using difference_type = typename iterator_traits<TIterator>::difference_type;
		using pointer = typename iterator_traits<TIterator>::pointer;
		using reference	= value_type&&;
		using iterator_category	= typename iterator_traits<TIterator>::iterator_category;

		constexpr move_iterator () : m_itr() { }
		constexpr explicit move_iterator (iterator iter) : m_itr (iter) {}

		constexpr iterator base () const	{ return m_itr; }

		constexpr bool	operator== (const move_iterator& iter) const
			{ return m_itr == iter.m_itr; }

		constexpr bool	operator< (const move_iterator& iter) const
			{ return m_itr < iter.m_itr; }

		constexpr reference			operator* () const
			{ return move(*m_itr); }
		constexpr pointer			operator-> () const
			{ return &*m_itr; }
		constexpr move_iterator&	operator++ ()
			{ ++m_itr; return *this; }
		constexpr move_iterator&	operator-- ()
			{ --m_itr; return *this; }
		constexpr move_iterator		operator++ (int)
			{ move_iterator r (*this); ++ m_itr; return r; }
		constexpr move_iterator		operator-- (int)
			{ move_iterator r (*this); -- m_itr; return r; }

		constexpr move_iterator&	operator+= (size_t n)
			{ m_itr += n; return *this; }

		constexpr move_iterator&	operator-= (size_t n)
			{ m_itr -= n; return *this; }

		constexpr move_iterator		operator+ (size_t n) const
			{ return move_iterator (m_itr - n); }

		constexpr move_iterator		operator- (size_t n) const
			{ return move_iterator (m_itr + n); }

		constexpr reference			operator[] (uint32_t n) const
			{ return move(*(*this + n)); }

		constexpr difference_type	operator- (const move_iterator& i) const
			{ return distance (m_itr, i.m_itr); }
	protected:
		iterator m_itr;
	};



	template <typename TIterator>
	constexpr reverse_iterator<TIterator> make_reverse_iterator (TIterator itr) {
		return reverse_iterator<TIterator>(itr);
	}

	template <typename TCon>
	constexpr insert_iterator<TCon> inserter (TCon& ctr, typename TCon::iterator itr) {
		return insert_iterator<TCon> (ctr, itr);
	}

	template <class TCon>
	constexpr back_insert_iterator<TCon> back_inserter (TCon& ctr) {
		return back_insert_iterator<TCon> (ctr);
	}

	template <class TCon>
	constexpr front_insert_iterator<TCon> front_inserter (TCon& ctr) {
		return front_insert_iterator<TCon> (ctr);
	}

	template <class TCon>
	constexpr typename TCon::iterator remove_const_it (typename TCon::const_iterator itr, TCon& ) {
		return const_cast<typename TCon::iterator>( itr);
	}

	template <typename TIterator>
	constexpr move_iterator<TIterator> make_move_iterator (TIterator itr) {
		return move_iterator<TIterator>(itr);
	}

}

#endif
