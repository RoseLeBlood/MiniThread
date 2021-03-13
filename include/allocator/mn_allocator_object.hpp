/** 
 * This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
 * Copyright (c) 2021 Amber-Sophia Schroeck
 *
 * The Mini Thread Library is free software; you can redistribute it and/or modify  
 * it under the terms of the GNU Lesser General Public License as published by  
 * the Free Software Foundation, version 3, or (at your option) any later version.

 * The Mini Thread Library is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the Mini Thread  Library; if not, see
 * <https://www.gnu.org/licenses/>.  
*/

#ifndef _MINLIB_ALLOCATOR_OBJCT_H_
#define _MINLIB_ALLOCATOR_OBJCT_H_

      
/**
 * @brief Defines when the class the mn::allocator to create the object, 
 * with new and delete.
 * 
 * @code
 * class foo() {
 *      MNALLOC_OBJECT(default_allocator_t);
 * public:
 *      foo() : m_iInt(0) { }
 * 
 *      int get() { return m_iInt; }
 * private:
 *      int m_iInt;
 * };
 * 
 * MNALLOC_OBJECT_D0(foo);
 * 
 * int main() {
 *      foo* _pFooo = new foo();
 *      std::cout << _pFooo->get() << std::endl;
 * 
 *      return 0;
 * }
 * @endcode
 */
#define MNALLOC_OBJECT(AlloCator) \
public: \
using allocator_type = AlloCator; \
private: \
static allocator_type m_acObject; \
public:  \
    void* operator new (unsigned int size) { return m_acObject.alloc(size ); }  \
    void* operator new[] (unsigned int size) { return m_acObject.alloc(size ); }  \
    void  operator delete(void* pObject) { m_acObject.free(pObject); } \
    void  operator delete[] (void* pObject) { m_acObject.free(pObject); } \
    allocator_type get_allocator() { return m_acObject; } \
    void set_allocator(allocator_type alloc) { m_acObject = alloc; } 
    
            
            
/**
 * @brief Helper macro to declar/define the static m_acObject member in a 
 * MNALLOC_OBJECT, for non templates
 */
#define MNALLOC_OBJECT_D0(Base)\
    typename Base::allocator_type Base::m_acObject;

/**
 * @brief Helper macro to declar/define the static m_acObject member in a 
 * MNALLOC_OBJECT.\n
 * for templates with one tparam 
 * 
 * @code
 * template <class TALLOCATOR>
 * class foo {
 *       MNALLOC_OBJECT(TALLOCATOR);
 * public:
 *      foo() { }
 * };
 * MNALLOC_OBJECT_D1(foo, class, TALLOCATOR);
 * 
 * ///ASS Struct
 * template <class TALLOCATOR>
 * struct foo {
 *  MNALLOC_OBJECT(TALLOCATOR);
 *  int Value;
 * };
 * MNALLOC_OBJECT_D1(foo, class, TALLOCATOR);
 * @endcode
 */
#define MNALLOC_OBJECT_DONE(Base,t1,d1) template<t1 d1> \
    typename Base<d1>::allocator_type Base<d1>::m_acObject;
/**
 * @brief @see MNALLOC_OBJECT_D1, becourse this macro is for 2 template parameter
 * @code
 * MNALLOC_OBJECT_D2(foo2, typename, T, class, TALLOCATOR );
 * MNALLOC_OBJECT_D2(foo2, typename, T, typename, U );
 * @endcode
 */
#define MNALLOC_OBJECT_DTWO(Base,t1,d1,t2,d2) template<t1 d1, t2 d2> \
    typename Base<d1,d2>::allocator_type Base<d1,d2>::m_acObject;

/**
 * @brief @see MNALLOC_OBJECT_D1, becourse this macro is for 2 template parameter
 * @code
 * MNALLOC_OBJECT_D2(foo2, typename, T, class, TALLOCATOR );
 * MNALLOC_OBJECT_D2(foo2, typename, T, typename, U );
 * @endcode
 */
#define MNALLOC_OBJECT_DTHREE(Base,t1,d1,t2,d2,t3,d3) template<t1 d1, t2 d2, t3 d3> \
    typename Base<d1, d2, d3>::allocator_type Base<d1, d2, d3>::m_acObject;       


#endif  