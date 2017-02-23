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

#include "SchemaVertex.h"

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/Tags.h>

namespace hoot
{

SchemaVertex::SchemaVertex()
{
  influence = -1.0;
  valueType = Unknown;
  childWeight = -1.0;
  mismatchScore = -1.0;
  geometries = 0;
  _type = UnknownVertexType;
}

void SchemaVertex::addCompoundRule(const CompoundRule& rule)
{
  if (_type != Compound)
  {
    throw IllegalArgumentException("Can only add compound KVPs to compound types.");
  }
  _compoundRules.append(rule);
}

bool SchemaVertex::isCompoundMatch(const Tags& t) const
{
  bool matchOne = false;

  // go through each rule
  for (int i = 0; i < _compoundRules.size() && matchOne == false; ++i)
  {
    CompoundRule r = _compoundRules[i];

    if (isMatch(r, t))
    {
      matchOne = true;
    }
  }

  return matchOne;
}

bool SchemaVertex::isMatch(const CompoundRule& rule, const Tags& t)
{
  vector<bool> matches(rule.size(), false);
  int matchCount = 0;

  for (Tags::const_iterator it = t.begin(); it != t.end(); ++it)
  {
    QString k = it.key();
    QString v = it.value();

    for (int i = 0; i < rule.size(); ++i)
    {
      if (matches[i] == false && rule[i]->isMatch(k, v))
      {
        matches[i] = true;
        ++matchCount;
      }
    }
  }

  return matchCount == rule.size();
}

void SchemaVertex::setType(VertexType t)
{
  if (t == Tag)
  {
    _compoundRules.clear();
  }
  _type = t;
}

void SchemaVertex::setName(QString n)
{
  name = n;
}

void SchemaVertex::setNameKvp(QString n)
{
  int equalsPos = n.indexOf('=');
  if (equalsPos == 0)
  {
    throw HootException("The name cannot start with an equals sign. " + n);
  }
  else if (equalsPos > 0)
  {
    key = n.left(equalsPos);
    value = n.mid(equalsPos + 1);
  }
  else
  {
    key = n;
    value.clear();
  }
  name = n;
}

void SchemaVertex::setValueTypeString(QString t)
{
  if (t == "enumeration")
  {
    valueType = Enumeration;
  }
  else if (t == "text")
  {
    valueType = Text;
  }
  else if (t == "int")
  {
    valueType = Int;
  }
  else if (t == "real")
  {
    valueType = Real;
  }
  else
  {
    throw HootException("Unexpected type tag: " + t);
  }
}

QString SchemaVertex::toString() const
{
  QString result = QString("name: %1\n").arg(name)
      + QString("key: %1\n").arg(key)
      + QString("value: %1\n").arg(value)
      + QString("influence: %1\n").arg(influence)
      + QString("childWeight: %1\n").arg(childWeight)
      + QString("mismatchScore: %1\n").arg(mismatchScore)
      + QString("valueType: %1\n").arg(valueType)
      + QString("aliases: %1\n").arg(hoot::toString(aliases))
      + QString("geometries: %1\n").arg(geometries)
      + QString("categories: %1\n").arg(hoot::toString(categories));

  if (_type == Compound)
  {
    result += QString("tags: %1\n").arg(hoot::toString(_compoundRules));
  }

  return result;
}


}
