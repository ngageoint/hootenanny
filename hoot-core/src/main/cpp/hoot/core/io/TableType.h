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
 * @copyright Copyright (C) 2016, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef TABLETYPE_H
#define TABLETYPE_H

// Hoot
#include <hoot/core/elements/ElementType.h>

namespace hoot
{

class TableType
{

public:

  // we could add the changesets table to this
  enum Type
  {
    Node = 0,
    Way = 1,
    Relation = 2,
    WayNode = 3,
    RelationMember = 4,
    Unknown = 5
  };

  TableType() { _type = Unknown; }
  TableType(Type type) { _type = type; }

  bool operator==(TableType t) const { return t._type == _type; }
  bool operator!=(TableType t) const { return t._type != _type; }

  Type getEnum() const { return _type; }

  static Type fromElementType(const ElementType& elementType)
  {
    switch (elementType.getEnum())
    {
      case ElementType::Node:
        return Node;
      case ElementType::Way:
        return Way;
      case ElementType::Relation:
        return Relation;
      default:
        throw HootException("Invalid element type.");
    }
  }

private:

  TableType::Type _type;

};

}

#endif // TABLETYPE_H
