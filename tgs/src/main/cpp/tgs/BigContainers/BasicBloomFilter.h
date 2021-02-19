/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef BASICBLOOMFILTER_H
#define BASICBLOOMFILTER_H

//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Alejandro Cabrera 2011.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bloom_filter for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_BLOOM_FILTER_BLOOM_FILTER_HPP
#define BOOST_BLOOM_FILTER_BLOOM_FILTER_HPP 1

#include <cmath>
#include <bitset>

#include <boost/config.hpp>
#include <boost/functional/hash.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>

#ifndef BOOST_NO_0X_HDR_INITIALIZER_LIST
#include <initializer_list>
#endif

// uses "constexpr" with C++11, otherwise it is omitted
#ifndef BOOST_CONSTEXPR
# define BLOOM_CONSTEXPR
#else
# define BLOOM_CONSTEXPR BOOST_CONSTEXPR
#endif

namespace boost
{
  namespace bloom_filters
  {
    namespace detail
    {
      template <size_t N,
                typename Container>
      struct apply_hash
      {
        typedef typename Container::value_type value_type;
        typedef typename Container::bitset_type bitset_type;
        typedef typename Container::hash_function_type hash_function_type;

        static void insert(const value_type& t,
                           bitset_type& _bits)
        {
          typedef typename boost::mpl::at_c<hash_function_type, N>::type Hash;
          static Hash hasher;

          _bits[hasher(t) % _bits.size()] = true;
          apply_hash<N-1, Container>::insert(t, _bits);
        }

        static bool contains(const value_type& t,
                             const bitset_type& _bits)
        {
          typedef typename boost::mpl::at_c<hash_function_type, N>::type Hash;
          static Hash hasher;

          //std::cout << "hash: " << N << " " << hasher(t) << std::endl;
          return (_bits[hasher(t) % _bits.size()] &&
                  apply_hash<N-1, Container>::contains(t, _bits));
        }
      };

      template <typename Container>
      struct apply_hash<0, Container>
      {
        typedef typename Container::value_type value_type;
        typedef typename Container::bitset_type bitset_type;
        typedef typename Container::hash_function_type hash_function_type;

        static void insert(const value_type& t,
                           bitset_type& _bits)
        {
          typedef typename boost::mpl::at_c<hash_function_type, 0>::type Hash;
          static Hash hasher;

          _bits[hasher(t) % _bits.size()] = true;
        }

        static bool contains(const value_type& t,
                             const bitset_type& _bits)
        {
          typedef typename boost::mpl::at_c<hash_function_type, 0>::type Hash;
          static Hash hasher;

          //std::cout << "hash: " << 0 << " " << hasher(t) << std::endl;
          return (_bits[hasher(t) % _bits.size()]);
        }
      };

    } // namespace detail

    template <typename T, size_t Seed = 0>
      struct boost_hash
      {
        size_t operator()(const T& t)
        {
          return boost::hash_value(t ^ Seed);
        }
      };


    template <typename T,
              size_t Size,
              class HashFunctions = mpl::vector<boost_hash<T>>>
    class basic_bloom_filter
    {
    public:
      typedef T value_type;
      typedef T key_type;
      typedef std::bitset<Size> bitset_type;
      typedef HashFunctions hash_function_type;
      typedef basic_bloom_filter<T, Size,
                                 HashFunctions> this_type;

    private:
      typedef detail::apply_hash<mpl::size<HashFunctions>::value - 1,
                                 this_type> apply_hash_type;

    public:
      basic_bloom_filter() : _count(0) {}

      template <typename InputIterator>
      basic_bloom_filter(const InputIterator start, const InputIterator end)
      {
        for (InputIterator i = start; i != end; ++i)
          this->insert(*i);
      }

#ifndef BOOST_NO_0X_HDR_INITIALIZER_LIST
      basic_bloom_filter(const std::initializer_list<T>& ilist)
      {
        typedef typename std::initializer_list<T>::const_iterator citer;
        for (citer i = ilist.begin(), end = ilist.end(); i != end; ++i)
        {
          this->insert(*i);
        }
      }
#endif

      static BLOOM_CONSTEXPR size_t bit_capacity()
      {
        return Size;
      }

      static BLOOM_CONSTEXPR size_t num_hash_functions()
      {
        return mpl::size<HashFunctions>::value;
      };

      double false_positive_rate() const
      {
        const double n = static_cast<double>(this->_count);
        static const double k = static_cast<double>(num_hash_functions());
        static const double m = static_cast<double>(Size);
        static const double e =
          2.718281828459045235360287471352662497757247093699959574966;
        return std::pow(1 - std::pow(e, -k * n / m), k);
      }

      size_t count() const
      {
        return this->_count;
      }

      bool empty() const
      {
        return this->count() == 0;
      }

      const bitset_type&
      data() const
      {
        return this->bits;
      }

      void insert(const T& t)
      {
        ++this->_count;
        apply_hash_type::insert(t, bits);
      }

      template <typename InputIterator>
      void insert(const InputIterator start, const InputIterator end)
      {
        for (InputIterator i = start; i != end; ++i)
        {
          this->insert(*i);
        }
      }

      bool probably_contains(const T& t) const
      {
        return apply_hash_type::contains(t, bits);
      }

      void clear()
      {
        this->_count = 0;
        this->bits.reset();
      }

      void swap(basic_bloom_filter& other)
      {
        basic_bloom_filter tmp = other;
        other = *this;
        *this = tmp;
      }

      basic_bloom_filter& operator|=(const basic_bloom_filter& rhs)
      {
        this->bits |= rhs.bits;
        return *this;
      }

      basic_bloom_filter& operator&=(const basic_bloom_filter& rhs)
      {
        this->bits &= rhs.bits;
        return *this;
      }

      template<class _T, size_t _Size, class _HashFunctions>
      friend bool
      operator==(const basic_bloom_filter<_T, _Size, _HashFunctions>&,
                 const basic_bloom_filter<_T, _Size, _HashFunctions>&);

      template<class _T, size_t _Size, class _HashFunctions>
      friend bool
      operator!=(const basic_bloom_filter<_T, _Size, _HashFunctions>&,
                 const basic_bloom_filter<_T, _Size, _HashFunctions>&);

    private:
      bitset_type bits;
      size_t _count;
    };

    template<class _T, size_t _Size, class _HashFunctions>
    bool
    operator==(const basic_bloom_filter<_T, _Size, _HashFunctions>& lhs,
               const basic_bloom_filter<_T, _Size, _HashFunctions>& rhs)
    {
      return (lhs.bits == rhs.bits);
    }

    template<class _T, size_t _Size, class _HashFunctions>
    bool
    operator!=(const basic_bloom_filter<_T, _Size, _HashFunctions>& lhs,
               const basic_bloom_filter<_T, _Size, _HashFunctions>& rhs)
    {
      return !(lhs == rhs);
    }

    template<class _T, size_t _Size, class _HashFunctions>
    basic_bloom_filter<_T, _Size, _HashFunctions>
    operator|(const basic_bloom_filter<_T, _Size, _HashFunctions>& lhs,
              const basic_bloom_filter<_T, _Size, _HashFunctions>& rhs)
    {
      basic_bloom_filter<_T, _Size, _HashFunctions> ret(lhs);
      ret |= rhs;
      return ret;
    }

    template<class _T, size_t _Size, class _HashFunctions>
    basic_bloom_filter<_T, _Size, _HashFunctions>
    operator&(const basic_bloom_filter<_T, _Size, _HashFunctions>& lhs,
              const basic_bloom_filter<_T, _Size, _HashFunctions>& rhs)
    {
      basic_bloom_filter<_T, _Size, _HashFunctions> ret(lhs);
      ret &= rhs;
      return ret;
    }

    template<class _T, size_t _Size, class _HashFunctions>
    void
    swap(basic_bloom_filter<_T, _Size, _HashFunctions>& lhs,
         basic_bloom_filter<_T, _Size, _HashFunctions>& rhs)
    {
      lhs.swap(rhs);
    }
  } // namespace bloom_filters
} // namespace boost
#endif

#endif // BASICBLOOMFILTER_H
