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
 * @copyright Copyright (C) 2015, 2016, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef ELEMENTTYPE_H
#define ELEMENTTYPE_H

// Hoot
#include <hoot/core/util/HootException.h>

// Qt


namespace hoot
{

class ElementType
{
public:

  enum Type
  {
    Node = 0,
    Way = 1,
    Relation = 2,
    Unknown = 3,
    Max = 3
  };

  ElementType() : _type(Unknown) { }
  ElementType(Type type) : _type(type) { }

  bool operator==(ElementType t) const { return t._type == _type; }
  bool operator!=(ElementType t) const { return t._type != _type; }

  Type getEnum() const { return _type; }

  QString toString() const
  {
    switch (_type)
    {
    case ElementType::Node:
      return "Node";
    case ElementType::Way:
      return "Way";
    case ElementType::Relation:
      return "Relation";
    default:
      return QString("Unknown (%1)").arg(_type);
    }
  }

  static bool isValidTypeString(QString typeString)
  {
    typeString = typeString.toLower();
    return typeString == "node" || typeString == "way" || typeString == "relation" ||
      typeString == "unknown";
  }

  static Type fromString(QString typeString)
  {
    typeString = typeString.toLower();
    if (typeString == "node")
    {
      return Node;
    }
    else if (typeString == "way")
    {
      return Way;
    }
    else if (typeString == "relation")
    {
      return Relation;
    }
    else if (typeString == "unknown")
    {
      return Unknown;
    }
    else
    {
      throw IllegalArgumentException("Invalid element type string: " + typeString);
    }
  }

private:

  ElementType::Type _type;
};

}

#endif // ELEMENTTYPE_H
