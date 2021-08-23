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
 * @copyright Copyright (C) 2015, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

#include "SqliteWordWeightDictionary.h"

// hoot


// Qt



namespace hoot
{

SqliteWordWeightDictionary::SqliteWordWeightDictionary(const QString& filePath) : _reader(filePath)
{
  _count = 1;
  _nonWord.setPattern("[^\\w]");
}

double SqliteWordWeightDictionary::getWeight(const QString& word) const
{
  LOG_VART(word);
  QString normalized = word.toLower().normalized(QString::NormalizationForm_C);
  normalized.replace(_nonWord, "");
  WeightHash::const_iterator it = _weights.find(normalized);

  long c;
  if (it == _weights.end())
  {
    c = _reader.readCount(normalized);
    if (c > 0)
    {
      _weights[normalized] = c;
    }
    return c;
  }
  else
  {
    c = it->second;
  }

  return c / (double)_count;
}

}
