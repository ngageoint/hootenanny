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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef POI_POLYGON_SCHEMA_TYPE_H
#define POI_POLYGON_SCHEMA_TYPE_H

// Hoot
#include <hoot/core/util/HootException.h>

// Qt
#include <QString>

namespace hoot
{

/**
 * TODO
 */
class PoiPolygonSchemaType
{

public:

  typedef enum Type
  {
    Natural = 0,
    Park,
    Parking,
    Parkish,
    Playground,
    Restaurant,
    Religion,
    Restroom,
    School,
    SpecificSchool,
    Sport
  } Type;

  PoiPolygonSchemaType(Type type) : _type(type) {}

  bool operator==(PoiPolygonSchemaType t) const { return t._type == _type; }
  bool operator!=(PoiPolygonSchemaType t) const { return t._type != _type; }

  Type getEnum() const { return _type; }

  QString toString() const
  {
    switch (_type)
    {
    case PoiPolygonSchemaType::Natural:
      return "Natural";
    case PoiPolygonSchemaType::Park:
      return "Park";
    case PoiPolygonSchemaType::Parking:
      return "Parking";
    case PoiPolygonSchemaType::Parkish:
      return "Parkish";
    case PoiPolygonSchemaType::Playground:
      return "Playground";
    case PoiPolygonSchemaType::Restaurant:
      return "Restaurant";
    case PoiPolygonSchemaType::Religion:
      return "Religion";
    case PoiPolygonSchemaType::Restroom:
      return "Restroom";
    case PoiPolygonSchemaType::School:
      return "School";
    case PoiPolygonSchemaType::SpecificSchool:
      return "SpecificSchool";
    case PoiPolygonSchemaType::Sport:
      return "Sport";
    default:
      return QString("Unknown (%1)").arg(_type);
    }
  }

  static Type fromString(QString typeString)
  {
    typeString = typeString.toLower().trimmed();
    if (typeString == "natural")
    {
      return Natural;
    }
    else if (typeString == "park")
    {
      return Park;
    }
    else if (typeString == "parkish")
    {
      return Parkish;
    }
    else if (typeString == "Playground")
    {
      return Playground;
    }
    else if (typeString == "restaurant")
    {
      return Restaurant;
    }
    else if (typeString == "religion")
    {
      return Religion;
    }
    else if (typeString == "restroom")
    {
      return Restroom;
    }
    else if (typeString == "school")
    {
      return School;
    }
    else if (typeString == "specificschool")
    {
      return SpecificSchool;
    }
    else if (typeString == "sport")
    {
      return Sport;
    }
    else
    {
      throw IllegalArgumentException("Invalid POI/Polygon schema type string: " + typeString);
    }
  }

private:

  PoiPolygonSchemaType::Type _type;

};

}

#endif // POI_POLYGON_SCHEMA_TYPE_H
