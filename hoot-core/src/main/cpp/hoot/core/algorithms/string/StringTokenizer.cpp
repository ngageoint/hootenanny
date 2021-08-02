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

#include "StringTokenizer.h"

// hoot
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

StringTokenizer::StringTokenizer()
{
  setConfiguration(conf());
}

StringTokenizer::StringTokenizer(const QString& sepRegex) : _sep(sepRegex)
{
}

bool StringTokenizer::_isNonWord(const QString& s) const
{
  for (int i = 0; i < s.size(); i++)
  {
    if (s.at(i).isLetterOrNumber())
    {
      return false;
    }
  }

  return true;
}

void StringTokenizer::setConfiguration(const Settings& conf)
{
  ConfigOptions co(conf);
  _sep.setPattern(co.getTokenSeparator());
  _keepNonWords = co.getTokenKeepNonWords();
  _minSize = co.getTokenMinSize();
}

QStringList StringTokenizer::tokenize(const QString& s) const
{
  QStringList l = s.split(_sep, QString::SkipEmptyParts);

  for (int i = 0; i < l.size();)
  {
    if (_keepNonWords == false && _isNonWord(l[i]))
    {
      l.removeAt(i);
    }
    else if (l[i].size() < _minSize)
    {
      l.removeAt(i);
    }
    else
    {
      i++;
    }
  }

  return l;
}

}
