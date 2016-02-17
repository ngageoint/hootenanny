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
#include "TagContainsFilter.h"

// hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

const QString TagContainsFilter::WILDCARD = "*";

TagContainsFilter::TagContainsFilter(FilterType type, QString key, QString valueSubstring)
{
  _type = type;
  _key.append(key);
  _valueSubstring.append(valueSubstring);
}

TagContainsFilter::TagContainsFilter(FilterType type, QString key)
{
  _type = type;
  _key.append(key);
  _valueSubstring.append(TagContainsFilter::WILDCARD);
}

void TagContainsFilter::addPair(QString key)
{
  addPair(key, TagContainsFilter::WILDCARD);
}

void TagContainsFilter::addPair(QString key, QString valueSubstring)
{
  _key.append(key);
  _valueSubstring.append(valueSubstring);
}

bool TagContainsFilter::isFiltered(const Element& e) const
{
  bool matches = false;

  for (int i = 0; i < _key.size(); i++)
  {
/*
    if (e.getTags().contains(_key[i]) &&    //  Contains the right key
        ( e.getTags()[_key[i]].contains(_valueSubstring[i]) ||  //  Value is equals
          e.getTags()[_key[i]].compare(TagContainsFilter::WILDCARD) == 0 ))   //  Wildcard
    {
      matches = true;
    }
*/
    if (e.getTags().contains(_key[i]))
    {
      if (e.getTags()[_key[i]].contains(_valueSubstring[i]))
        matches = true;
      else if (_valueSubstring[i] == TagContainsFilter::WILDCARD)
        matches = true;
    }
  }

  if (_type == KeepMatches)
  {
    return !matches;
  }
  else
  {
    return matches;
  }
}

}
