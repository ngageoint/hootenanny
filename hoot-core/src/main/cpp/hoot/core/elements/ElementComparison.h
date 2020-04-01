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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef ELEMENT_COMPARISON_H
#define ELEMENT_COMPARISON_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/ElementComparer.h>

namespace hoot
{

/**
 * This is an abstraction for dealing with element comparisons inside of collections.
 *
 * This is useful because ElementComparer has evolved as the way to compare elements vs using ==
 * operators. Additionally, there is also a hash visitor available for nodes only. At some point we
 * may want to look into consolidating all of this behavior into the Element == operators, if
 * possible.
 */
class ElementComparison
{
public:

  ElementComparison();
  ElementComparison(ElementPtr element, const OsmMap& sourceMap,
                    const bool ignoreElementId = false);

  ElementPtr getElement() const { return _element; }

  virtual bool operator==(const ElementComparison& elementComp) const;

  virtual QString toString() const;

  bool isNull() const { return !_element.get(); }

protected:

  ElementPtr _element;
  ElementComparer _elementComparer;
};

inline uint qHash(const ElementComparison& elementComp)
{
  return qHash(elementComp.getElement()->nonIdHash());
}

}

#endif // ELEMENT_COMPARISON_H
