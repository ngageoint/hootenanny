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
 * @copyright Copyright (C) 2015, 2017, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef HOOT_HYBRID_NODE_H
#define HOOT_HYBRID_NODE_H

// Qt
#include <QString>

// Standard
#include <sstream>

namespace hoot
{

namespace hybrid
{

template <class KeyType, class DataType>
class Node
{
public:

  virtual ~Node() = default;

  virtual const Node<KeyType, DataType>* getChild(size_t i) const = 0;

  virtual size_t getChildCount() const = 0;

  virtual QString toString(QString indent = QString()) const  = 0;

};

#define LEAF_NODE_SIZE 2

template <class KeyType, class DataType>
class Leaf : public Node<KeyType, DataType>
{
public:

  Leaf()
  {
    _size = 0;
    _next = 0;
  }
  virtual ~Leaf()
  {
    delete _next;
  }

  void addData(const KeyType& k, const DataType& d)
  {
    if (_next != 0)
    {
      _next->addData(k, d);
    }
    else
    {
      if (_size < LEAF_NODE_SIZE)
      {
        _elements[(size_t)_size] = std::pair<KeyType, DataType>(k, d);
        ++_size;
      }
      else
      {
        _next = new Leaf();
        _next->addData(k, d);
      }
    }
  }

  const std::pair<KeyType, DataType>& get(size_t i) const
  {
    if (i < (size_t)_size)
    {
      return _elements[i];
    }
    else
    {
      return _next->get(i - LEAF_NODE_SIZE);
    }
  }

  virtual const Node<KeyType, DataType>* getChild(size_t) const { return 0; }

  virtual size_t getChildCount() const { return 0; }

  size_t getSize() const
  {
    size_t result = _size;
    if (_next != 0)
    {
      result += _next->getSize();
    }
    return result;
  }

  virtual QString toString(QString indent = QString()) const
  {
    std::stringstream ss;
    ss << indent.toUtf8().data() << "Leaf: " << std::endl;
    size_t c = getSize();
    for (size_t i = 0; i < c; ++i)
    {
      ss << indent.toUtf8().data() << "  " << get(i).first << " : " << get(i).second << std::endl;
    }
    return QString::fromUtf8(ss.str().data());
  }

private:
  char _size;
  std::pair<KeyType, DataType> _elements[LEAF_NODE_SIZE];
  Leaf* _next;
};

}

}

#endif // HOOT_HYBRID_NODE_H
