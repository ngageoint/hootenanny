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
 * @copyright Copyright (C) 2015-2023 Maxar (http://www.maxar.com/)
 */

#ifndef COLLECTION_UTILS_H
#define COLLECTION_UTILS_H

namespace hoot
{

/**
 * General utilities needed when working with collections
 */
class CollectionUtils
{

public:

  /**
   * Flips a map, making its keys the values and its values the keys
   *
   * @param src a map
   * @return a flipped map
   */
  template<typename A, typename B>
  static std::multimap<B, A> flipMap(const std::map<A, B>& src)
  {
    std::multimap<B, A> dst;
    std::transform(src.begin(), src.end(), std::inserter(dst, dst.begin()), _flipPair<A, B>);
    return dst;
  }

  /**
   * Converts a stdlib set to a Qt set
   *
   * @param set the set to convert
   * @return a Qt set
   * @note std::set is ordered and QSet is not
   */
  template<typename T>
  static QSet<T> stdSetToQSet(const std::set<T>& s)
  {
    // There's probably a more efficient way to do this...
    QSet<T> qSet;
    for (auto itr = s.begin(); itr != s.end(); ++itr)
      qSet.insert(*itr);
    return qSet;
  }

  /**
   * Converts a Qt set to a stdlib set
   *
   * @param set the set to convert
   * @return a stdlib set
   * @note std::set is ordered and QSet is not
   */
  template<typename T>
  static std::set<T> qSetToStdSet(const QSet<T>& s)
  {
    // There's probably a more efficient way to do this...
    std::set<T> stdSet;
    for (auto itr = s.begin(); itr != s.end(); ++itr)
      stdSet.insert(*itr);
    return stdSet;
  }

private:

  template<typename A, typename B>
  static std::pair<B,A> _flipPair(const std::pair<A,B>& p)
  {
    return std::pair<B, A>(p.second, p.first);
  }
};

}

#endif // COLLECTION_UTILS_H
