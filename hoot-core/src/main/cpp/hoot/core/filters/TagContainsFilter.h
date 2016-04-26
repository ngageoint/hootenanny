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
#ifndef TAGCONTAINSFILTER_H
#define TAGCONTAINSFILTER_H

// Qt
#include <QStringList>

#include "ElementCriterion.h"

namespace hoot
{

class TagContainsFilter : public BaseElementFilter
{
public:
  TagContainsFilter(FilterType type, QString key, QString valueSubstring);
  TagContainsFilter(FilterType type, QStringList keys, QStringList valueSubstrings);

  /**
   * Adds an additional pair to the search list. If any one of the pairs matches then it is
   * considered a match.
   */
  void addPair(QString key, QString valueSubstring);

  virtual ElementCriterion* clone() { return new TagContainsFilter(_type, _key, _valueSubstring); }

protected:
  virtual bool isFiltered(const Element& e) const;

  QStringList _key, _valueSubstring;
  FilterType _type;
};

}

#endif // TAGCONTAINSFILTER_H
