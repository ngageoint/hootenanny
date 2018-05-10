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
 * @copyright Copyright (C) 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "StringUtils.h"

// Qt
#include <QLocale>

namespace hoot
{

QString StringUtils::secondsToDhms(const qint64 durationInMilliseconds)
{
  QString res;
  int duration = (int)(durationInMilliseconds / 1000);
  const int seconds = (int)(duration % 60);
  duration /= 60;
  const int minutes = (int)(duration % 60);
  duration /= 60;
  const int hours = (int)(duration % 24);
  const int days = (int)(duration / 24);
  if ((hours == 0) && (days == 0))
  {
    return res.sprintf("%02d:%02d", minutes, seconds);
  }
  if (days == 0)
  {
    return res.sprintf("%02d:%02d:%02d", hours, minutes, seconds);
  }
  return res.sprintf("%dd%02d:%02d:%02d", days, hours, minutes, seconds);
}

QString StringUtils::formatLargeNumber(const unsigned long number)
{
  //I want to see comma separators...probably a better way to handle this...will go with this for
  //now.
  const QLocale& cLocale = QLocale::c();
  QString ss = cLocale.toString((qulonglong)number);
  ss.replace(cLocale.groupSeparator(), ',');
  return ss;
}

bool StringUtils::hasAlphabeticCharacter(const QString input)
{
  for (int i = 0; i < input.length(); i++)
  {
    if (input.at(i).isLetter())
    {
      return true;
    }
  }
  return false;
}

}
