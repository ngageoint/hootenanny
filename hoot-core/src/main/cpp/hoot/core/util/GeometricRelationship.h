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
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef GEOMETRIC_RELATIONSHIP_H
#define GEOMETRIC_RELATIONSHIP_H

// Hoot
#include <hoot/core/util/HootException.h>

// Qt
#include <QString>

namespace hoot
{

/**
 * Enumeration mirroring the types of geometric comparisons availabe from geos::geom::Geometry
 */
class GeometricRelationship
{

public:

  typedef enum Type
  {
    Contains = 0,
    Covers,
    Crosses,
    DisjointWith,
    Intersects,
    IsWithin,
    Overlaps,
    Touches
  } Type;

  GeometricRelationship() : _type(Contains) {}
  GeometricRelationship(Type type) : _type(type) {}

  bool operator==(GeometricRelationship t) const { return t._type == _type; }
  bool operator!=(GeometricRelationship t) const { return t._type != _type; }

  Type getEnum() const { return _type; }

  QString toString() const
  {
    switch (_type)
    {
    case GeometricRelationship::Contains:
      return "Contains";
    case GeometricRelationship::Covers:
      return "Covers";
    case GeometricRelationship::Crosses:
      return "Crosses";
    case GeometricRelationship::DisjointWith:
      return "DisjointWith";
    case GeometricRelationship::Intersects:
      return "Intersects";
    case GeometricRelationship::IsWithin:
      return "IsWithin";
    case GeometricRelationship::Overlaps:
      return "Overlaps";
    case GeometricRelationship::Touches:
      return "Touches";
    default:
      return QString("Unknown (%1)").arg(_type);
    }
  }

  static Type fromString(QString typeString)
  {
    typeString = typeString.toLower().trimmed();
    if (typeString == "contains")
    {
      return Contains;
    }
    else if (typeString == "covers")
    {
      return Covers;
    }
    else if (typeString == "crosses")
    {
      return Crosses;
    }
    else if (typeString == "disjointWith")
    {
      return DisjointWith;
    }
    else if (typeString == "intersects")
    {
      return Intersects;
    }
    else if (typeString == "isWithin")
    {
      return IsWithin;
    }
    else if (typeString == "overlaps")
    {
      return Overlaps;
    }
    else if (typeString == "touches")
    {
      return Touches;
    }
    else
    {
      throw IllegalArgumentException("Invalid geometric relationship type string: " + typeString);
    }
  }

private:

  GeometricRelationship::Type _type;
};

}

#endif // GEOMETRIC_RELATIONSHIP_H
