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
#ifndef TAGMERGER_H
#define TAGMERGER_H

// hoot
#include <hoot/core/elements/ElementType.h>
#include <hoot/core/elements/Tags.h>

namespace hoot
{

/**
 * Interface for merging two sets of tags into one set of tags. This is most useful when conflating
 * two different features into a single feature.
 */
class TagMerger
{
public:

  static string className() { return "hoot::TagMerger"; }

  TagMerger() {}

  virtual ~TagMerger() {}

  virtual Tags mergeTags(const Tags& t1, const Tags& t2, ElementType et) const = 0;
};

typedef boost::shared_ptr<TagMerger> TagMergerPtr;
typedef boost::shared_ptr<const TagMerger> ConstTagMergerPtr;

}

#endif // TAGMERGER_H
