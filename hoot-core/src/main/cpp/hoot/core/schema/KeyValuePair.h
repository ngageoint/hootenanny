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
 * @copyright Copyright (C) 2015, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef KEYVALUEPAIR_H
#define KEYVALUEPAIR_H

// Hoot
#include <hoot/core/util/HootException.h>

// Qt
#include <QString>

namespace hoot
{

class KeyValuePair
{
public:
  KeyValuePair(QString kvp)
  {
    // this is faster than using split
    int index = kvp.indexOf('=');

    if (index == -1)
    {
      throw IllegalArgumentException("The key value pair must contain an equal sign.");
    }

    _name = kvp;
    _key = kvp.left(index);
    _value = kvp.mid(index + 1);

    // not supported at this time.
    if (_key.startsWith("regex?") || _value.startsWith("regex?"))
    {
      throw IllegalArgumentException("A key value pair cannot be a regular expression.");
    }
  }

  QString getKey() const { return _key; }
  QString getName() const { return _name; }
  QString getValue() const { return _value; }

  bool isMatch(const QString& k, const QString& v) { return _key == k && _value == v; }

  QString toString() const { return _name; }

private:
  QString _name;
  QString _key;
  QString _value;
};

typedef std::shared_ptr<KeyValuePair> KeyValuePairPtr;
typedef std::shared_ptr<const KeyValuePair> ConstKeyValuePairPtr;

}

#endif // KEYVALUEPAIR_H
