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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef POIIMPLICITTAGRULESDERIVER_H
#define POIIMPLICITTAGRULESDERIVER_H

// Qt
#include <QString>
#include <QMap>

namespace hoot
{

class Tags;

/**
 *
 */
class PoiImplicitTagRulesDeriver
{

public:

  //typedef QMap<QString, QMap<QString, long> > ImplicitTagRules;

  PoiImplicitTagRulesDeriver();

  QMap<QString, QMap<QString, long> > deriveRules(const QStringList inputs,
                                                  const QStringList typeKeys = QStringList(),
                                                  const int minOccurances = 1);

private:

  //ImplicitTagRules _tokensToKvpsWithCounts;
  //TODO: replace with stxxl map
  QMap<QString, QMap<QString, long> > _tokensToKvpsWithCounts;

  int _minOccurances;

  void _updateForNewToken(const QString token, const QString kvp);
  QString _getMostSpecificPoiKvp(const Tags& tags) const;
  void _removeKvpsBelowOccuranceThreshold();
};

}

#endif // POIIMPLICITTAGRULESDERIVER_H
