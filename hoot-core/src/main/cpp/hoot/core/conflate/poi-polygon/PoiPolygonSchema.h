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
#ifndef POI_POLYGON_SCHEMA_H
#define POI_POLYGON_SCHEMA_H

// Hoot
#include <hoot/core/elements/Element.h>

// Qt
#include <QMultiHash>

namespace hoot
{

/**
 * TODO
 */
class PoiPolygonSchema
{

public:

  PoiPolygonSchema();

  // As part of #2633, attempted to re-implement some of this hardcoded type code as categories in
  // the hoot schema.  In doing that, several strange bugs started occurring and many poi/poly unit
  // tests started to break.  Using the categories in that manner may not be the best approach and
  // possibly a different on is needed.  The branch "2633-new-categories" is an example of the failed
  // changes.

  /**
   * Determines if an element is a park
   *
   * @param element the element to examine
   * @return true if it is a park; false otherwise
   */
  static bool isPark(const ConstElementPtr& element);
  static bool isParkish(const ConstElementPtr& element);
  static bool isPlayground(const ConstElementPtr& element);
  static bool isSport(const ConstElementPtr& element);
  static bool isRestroom(const ConstElementPtr& element);
  static bool isParking(const ConstElementPtr& element);
  static bool isSchool(const ConstElementPtr& element);
  static bool isSpecificSchool(const ConstElementPtr& element);
  static bool specificSchoolMatch(const ConstElementPtr& element1, const ConstElementPtr& element2);
  static bool isReligion(const ConstElementPtr& element);
  static bool isRestaurant(const ConstElementPtr& element);
  static bool isNatural(const ConstElementPtr& element);

  /**
   * Determines if an element has more than one type associated with it
   *
   * @param element the element to examine
   * @return true if it has more than one type; false otherwise
   */
  static bool hasMoreThanOneType(const ConstElementPtr& element);

  /**
   * Determines if an element has a type associated with it
   *
   * @param element the element to examine
   * @return true if it has a type; false otherwise
   */
  static bool hasRelatedType(const ConstElementPtr& element);

  static bool hasSpecificType(const ConstElementPtr& element);

private:

  //maps an OSM kvp to multiple possible strings such a feature's name might contain
  static QMultiHash<QString, QString> _typeToNames;
  static QSet<QString> _allTagKeys;

  static void _readTypeToNames();
  static bool _typeHasName(const QString& kvp, const QString& name);
  static QString _getMatchingTypeName(const QString& kvp, const QString& name);
  static bool _haveMatchingTypeNames(const QString& kvp, const QString& name1, const QString& name2);
};

}

#endif // POI_POLYGON_SCHEMA_H
