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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef POIPOLYGONTYPEMATCH_H
#define POIPOLYGONTYPEMATCH_H

// hoot
#include <hoot/core/elements/Element.h>

namespace hoot
{

/**
 * Scores element type similarity
 */
class PoiPolygonTypeMatch
{
public:

  PoiPolygonTypeMatch(double typeScoreThreshold);

  /**
   * Returns a score from 0 to 1 representing the similarity of the feature types.
   */
  double getTypeScore(ConstElementPtr e1, ConstElementPtr e2, QString& t1BestKvp,
                      QString& t2BestKvp);
  double getTypeScore(ConstElementPtr e1, ConstElementPtr e2);
  bool isExactTypeMatch(ConstElementPtr e1, ConstElementPtr e2, QString& t1BestKvp,
                      QString& t2BestKvp);
  bool isExactTypeMatch(ConstElementPtr e1, ConstElementPtr e2);
  bool isTypeMatch(ConstElementPtr e1, ConstElementPtr e2, QString& t1BestKvp,
                   QString& t2BestKvp);
  bool isTypeMatch(ConstElementPtr e1, ConstElementPtr e2);

  static bool isPark(ConstElementPtr element);
  static bool isParkish(ConstElementPtr element);
  static bool isRecCenter(ConstElementPtr element);
  static bool isPlayground(ConstElementPtr element);
  static bool isPlayArea(ConstElementPtr element);
  static bool isSport(ConstElementPtr element);
  static bool isBuildingIsh(ConstElementPtr element);
  static bool hasMoreThanOneType(ConstElementPtr element);
  static bool hasType(ConstElementPtr element);

private:

  double _typeScoreThreshold;
  static QSet<QString> _allTagKeys;

  double _getTagScore(ConstElementPtr e1, ConstElementPtr e2, QString& t1BestKvp,
                      QString& t2BestKvp);
  QStringList _getRelatedTags(const Tags& tags) const;

};

}

#endif // POIPOLYGONTYPEMATCH_H
