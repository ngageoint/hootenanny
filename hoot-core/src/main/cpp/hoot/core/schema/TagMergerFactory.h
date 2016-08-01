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
#ifndef TAGMERGERFACTORY_H
#define TAGMERGERFACTORY_H

#include "TagMerger.h"
#include "AverageTagMerger.h"

// Tgs
#include <tgs/SharedPtr.h>

namespace hoot
{

class TagMergerFactory
{
public:

  TagMergerFactory();

  ~TagMergerFactory();

  static TagMergerFactory& getInstance();

  /**
   * Returns the default TagMerger.
   */
  const TagMerger& getDefault() { return *getDefaultPtr(); }

  shared_ptr<const TagMerger> getDefaultPtr();

  const TagMerger& getMerger(const QString& name) { return *getMergerPtr(name); }

  shared_ptr<const TagMerger> getMergerPtr(const QString& name);

  /**
   * A convenience function for merging tags using the default mechanism. Equivalent to:
   * TagMergerFactory::getInstance().getDefault().mergeTags(t1, t2, et);
   */
  static Tags mergeTags(const Tags& t1, const Tags& t2, ElementType et);

  /**
   * Reset all cached information. This is necessary if the default tag config information changes.
   */
  void reset();

private:
  QHash<QString, shared_ptr<const TagMerger> > _mergers;
  shared_ptr<const TagMerger> _default;

  static shared_ptr<TagMergerFactory> _theInstance;
};

}

#endif // TAGMERGERFACTORY_H
