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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef ELEMENTCOMPARER_H
#define ELEMENTCOMPARER_H

// Hoot
#include <hoot/core/elements/Element.h>

namespace hoot
{

/**
 * Compares two elements of the same type for similarity
 */
class ElementComparer
{

public:

  static const long DEBUG_ID = 0;

  explicit ElementComparer(Meters threshold = 0.05);

  bool isSame(ElementPtr e1, ElementPtr e2) const;

private:

  //currently, this threshold applies only to non-node circular error checks and the var would
  //eventually go away completely if all element types were converted over to uses hashes for
  //comparisons
  Meters _threshold;

  bool _compareNode(const std::shared_ptr<const Element>& re,
                    const std::shared_ptr<const Element>& e) const;
  bool _compareWay(const std::shared_ptr<const Element>& re,
                   const std::shared_ptr<const Element>& e) const;
  bool _compareRelation(const std::shared_ptr<const Element>& re,
                        const std::shared_ptr<const Element>& e) const;

  void _removeTagsNotImportantForComparison(Tags& tags) const;
};

}

#endif // ELEMENTCOMPARER_H
