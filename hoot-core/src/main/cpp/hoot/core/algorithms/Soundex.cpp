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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "Soundex.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QRegExp>

// Standard
#include <assert.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(StringDistance, Soundex);

Soundex::Soundex()
{
}

double Soundex::compare(const QString& s1, const QString& s2) const
{
  return (double)compareSoundex(s1, s2) / 4.0;
}

int Soundex::compareSoundex(QString word1, QString word2)
{
  return compareEncoded(encode(word1), encode(word2));
}

int Soundex::compareEncoded(QString encoded1, QString encoded2)
{
  assert(encoded1.size() == 4 && encoded2.size() == 4);
  int result = 4;

  for (int i = 0; i < 4; i++)
  {
    if (encoded1[i] != encoded2[i])
    {
      result--;
    }
  }

  return result;
}

QString Soundex::encode(QString s)
{
  QString result;
  s = s.toUpper();

  QString lastDigit = toDigit(s[0]);
  // keep the first character
  result.push_back(s[0]);
  s.remove(0, 1);

  QRegExp vowels("[AEIOUY]");

  // drop all other occurances of a, e, i, o, u, y, h, w
  s.replace(QRegExp("[HW]"), "");

  while (result.length() < 4)
  {
    QString d = toDigit(s[0]);
    if (s.size() == 0)
    {
      result.push_back("0");
    }
    // if the next character is a vowel
    else if (vowels.exactMatch(s.left(1)))
    {
      // remove it
      s.remove(0, 1);
    }
    else
    {
      s.remove(0, 1);
      if (lastDigit != d)
      {
        result.push_back(d);
      }
    }
    lastDigit = d;
  }

  return result;
}

QString Soundex::toDigit(QChar c)
{
  char a = c.toAscii();
  switch (a)
  {
  case 'B':
  case 'F':
  case 'P':
  case 'V':
    return "1";
  case 'C':
  case 'G':
  case 'J':
  case 'K':
  case 'Q':
  case 'S':
  case 'X':
  case 'Z':
    return "2";
  case 'D':
  case 'T':
    return "3";
  case 'L':
    return "4";
  case 'M':
  case 'N':
    return "5";
  case 'R':
    return "6";
  default:
    return "";
  }
}

}
