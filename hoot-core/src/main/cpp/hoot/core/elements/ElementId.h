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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef ELEMENTID_H
#define ELEMENTID_H

// Hoot
#include <hoot/core/elements/ElementType.h>

// Standard
#include <limits>

// tgs
#include <tgs/HashMap.h>

namespace hoot
{

/**
 * The ElementId uniquely identifies an element within an OsmMap. i.e. ElementType/id combination.
 *
 * This is a relatively new representation within hootenanny. In most cases where an ElementType/id
 * combo is found it should be replaced with the ElementId construct. There are also some places
 * where the elementType is implicit. Generally when performance/memory usage aren't an issue the
 * implicit element type / id combo should be replaced with an ElementId.
 */
class ElementId
{
public:

  static QString className() { return "ElementId"; }

  ElementId();
  ElementId(ElementType type, long id);
  ElementId(QString str);

  long getId() const { return _id; }

  ElementType getType() const { return _type; }

  bool isNull() const;

  bool operator!=(const ElementId& other) const;
  bool operator==(const ElementId& other) const;
  bool operator<(const ElementId& other) const;

  QString toString() const;
  QString toString();

  /**
   * Shorthand for ElementId(ElementType::Node, nid)
   */
  static ElementId node(long nid) { return ElementId(ElementType::Node, nid); }
  /**
   * Shorthand for ElementId(ElementType::Relation, rid)
   */
  static ElementId relation(long rid) { return ElementId(ElementType::Relation, rid); }
  /**
   * Shorthand for ElementId(ElementType::Way, wid)
   */
  static ElementId way(long wid) { return ElementId(ElementType::Way, wid); }

private:

  ElementType _type;
  long _id;
};

/**
 * A handy output stream method to enable debugging such as LOG_INFO("My eid: " << eid);
 */
inline std::ostream& operator<<(std::ostream& o, const ElementId& eid)
{
  o << eid.toString().toStdString();
  return o;
}

/**
 * The qHash function supports Qt's QHash collection class.
 */
inline uint qHash(const ElementId& eid)
{
  // XOR the enum values with the top bits in the id. The top bits in the id are usually empty, or
  // FFFF if it is a negative value.
  long key = eid.getId() ^ ((long)eid.getType().getEnum() << 58);
  // use Qt's function for hashing a long.
  return uint(((key >> (8 * sizeof(uint) - 1)) ^ key) & (~0U));
}

inline uint qHash(const std::pair<ElementId, ElementId>& peid)
{
  return static_cast<uint>(Tgs::cantorPairing(qHash(peid.first), qHash(peid.second)));
}

}

namespace __gnu_cxx
{
/**
 * Explicit template specialization of hash of an ElementId
 */
template <>
struct hash<hoot::ElementId>
{
  size_t operator() (const hoot::ElementId& eid) const
  {
    return size_t(eid.getId()) ^ ((size_t)eid.getType().getEnum() << 58);
  }
};

}

#endif // ELEMENTID_H
