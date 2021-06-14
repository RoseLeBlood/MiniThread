/**
 * @file
 * This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
 * @author Copyright (c) 2017 Amber-Sophia Schroeck
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

#ifndef _MINLIB_MN_FUNCTION_LIGHT_H_
#define _MINLIB_MN_FUNCTION_LIGHT_H_

#include "mn_config.hpp"

#include "mn_functional.hpp"
#include "utils/mn_alignment.hpp"

#include <type_traits>


namespace mn {

	template <class Sig, size_t sz, size_t algn>
    class small_task;

	template <class R, class... Args, size_t sz, size_t algn>
    class small_task<R(Args...), sz, algn> {
        struct vtable_t {
            void (*mover)(void *src, void *dest);

            void (*destroyer)(void *);

            R (*invoke)(void const *t, Args &&...args);

            template <class T> static vtable_t const *get() {
                static const vtable_t table = {
                    [](void *src, void *dest) { new (dest) T(move(*static_cast<T *>(src))); },
                    [](void *t) { static_cast<T *>(t)->~T(); },
                    [](void const *t, Args &&...args) -> R {
                        return (*static_cast<T const *>(t))(forward<Args>(args)...);
                    }
				};
                return &table;
            }
        };
	public:
		using return_type = R;

        small_task() { }
        small_task(const small_task &o)
            : table(o.table), data(o.data) { }

        small_task(small_task &&o)
            : table(o.table) {
            if (table) table->mover(&o.data, &data);
        }

        template <class F, class dF = decay_t<F>, enable_if_t<!is_same<dF, small_task>{}> * = nullptr,
                  enable_if_t<is_convertible<res_of_t<dF &(Args...)>, R>{}> * = nullptr>
        small_task(F &&f) : table(vtable_t::template get<dF>()) {
            static_assert(sizeof(dF) <= sz, "object too large");
            static_assert(alignof(dF) <= algn, "object too aligned");
            new (&data) dF(forward<F>(f));
        }
        ~small_task() {
            if (table) table->destroyer(&data);
        }


        small_task &operator=(const small_task &o) {
            this->~small_task();
            new (this) small_task(move(o));
            return *this;
        }

        small_task &operator=(small_task &&o) {
            this->~small_task();
            new (this) small_task(move(o));
            return *this;
        }

        explicit operator bool() const {
            return table;
        }

        return_type operator()(Args... args) const {
            return table->invoke(&data, mn::forward<Args>(args)...);
        }
	private:
		vtable_t const *table = nullptr;
        aligned_storage_t<sz, algn> data;
    };

    template <class R, class... Args, size_t sz, size_t algn>
    inline bool operator==(const small_task<R(Args...), sz, algn> &__f, nullptr_t) {
        return !static_cast<bool>(__f);
    }


    template <class R, class... Args, size_t sz, size_t algn>
    inline bool operator==(nullptr_t, const small_task<R(Args...), sz, algn> &__f) {
        return !static_cast<bool>(__f);
    }

    template <class R, class... Args, size_t sz, size_t algn>
    inline bool operator!=(const small_task<R(Args...), sz, algn> &__f, nullptr_t) {
        return static_cast<bool>(__f);
    }

    template <class R, class... Args, size_t sz, size_t algn>
    inline bool operator!=(nullptr_t, const small_task<R(Args...), sz, algn> &__f) {
        return static_cast<bool>(__f);
    }

    template <class Sig>
    using function = small_task<Sig, sizeof(void *) * 4, alignof(void *)>;

}

#include <functional>

#endif
