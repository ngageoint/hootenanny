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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef ELEMENT_COMPARISON_H
#define ELEMENT_COMPARISON_H

// Hoot
#include <hoot/core/elements/ElementComparer.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/schema/MetadataTags.h>

namespace hoot
{

/**
 * Allows for comparing elements within a collection
 *
 * @see ElementComparer
 */
class ElementComparison
{
public:

  ElementComparison() = default;
  ElementComparison(ElementPtr element, const OsmMap& sourceMap,
                    const bool ignoreElementId = false);
  virtual ~ElementComparison() = default;

  ElementPtr getElement() const { return _element; }

  virtual QString toString() const;

  bool isNull() const { return !_element.get(); }

  /**
   * Wrapper around ElementHashVisitor::toHashString
   */
  QString toHashString() const { return _elementComparer.toHashString(_element); }

protected:

  // This isn't const due to ElementComparer needing to add the hash to it before comparison.
  ElementPtr _element;
  ElementComparer _elementComparer;
};

inline uint qHash(const ElementComparison& elementComp)
{
  // reuse the hash if its already there
  const QString hashFromTag = elementComp.getElement()->getTags().get(MetadataTags::HootHash()).trimmed();
  if (!hashFromTag.isEmpty())
    return qHash(hashFromTag);
  return qHash(elementComp.toHashString());
}

}

#endif // ELEMENT_COMPARISON_H
