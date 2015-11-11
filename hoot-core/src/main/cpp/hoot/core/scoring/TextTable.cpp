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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "TextTable.h"

// hoot
#include <hoot/core/util/Log.h>

// Qt
#include <QSet>
#include <QStringList>

// Standard
#include <algorithm>

namespace hoot
{
using namespace std;

QStringList TextTable::_calculateColumns()
{
  QStringList columns;
  for (Data::const_iterator it = _data.begin(); it != _data.end(); ++it)
  {
    for (QHash<QString, QVariant>::const_iterator jt = it.value().begin(); jt != it.value().end();
         ++jt)
    {
      if (columns.contains(jt.key()) == false)
      {
        columns.append(jt.key());
      }
      _columnWidth[jt.key()] = max(_columnWidth[jt.key()], jt.key().size());
      _columnWidth[jt.key()] = max(_columnWidth[jt.key()], jt.value().toString().length());
    }
  }

  columns.sort();
  return columns;
}

QStringList TextTable::_calculateRows()
{
  QStringList result(_data.keys());

  _rowNameWidth = 0;
  for (int i = 0; i < result.size(); i++)
  {
    _rowNameWidth = max(_rowNameWidth, result[i].length());
  }

  result.sort();

  return result;
}

QString TextTable::toWikiString()
{
  QStringList rows = _calculateRows();
  QStringList cols = _calculateColumns();

  QString result;
  QString rowTitle;

  rowTitle = "| " + QString().fill(' ', _rowNameWidth + 1);
  rowTitle += "| ";
  for (int i = 0; i < _columnWidth.size(); i++)
  {
    rowTitle += cols[i] + " | ";
  }
  result = "\n" + rowTitle + "\n";

  for (int i = 0; i < rows.size(); i++)
  {
    QString rowString = "| " + QString("%1").arg(rows[i], _rowNameWidth) + " | ";

    for (int j = 0; j < cols.size(); j++)
    {
      QString cell = QString("%1").arg(_data[rows[i]][cols[j]].toString(), _columnWidth[cols[j]]);
      rowString += cell + " | ";
    }

    result += rowString + "\n";
  }

  return result;
}

}
