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
 * @copyright Copyright (C) 2013, 2014, 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef TAGMERGERFACTORY_H
#define TAGMERGERFACTORY_H

#include "TagMerger.h"
#include "AverageTagMerger.h"

namespace hoot
{

class TagMergerFactory
{
public:

  TagMergerFactory();

  ~TagMergerFactory();

  static TagMergerFactory& getInstance() { return _theInstance; }

  /**
   * Returns the default TagMerger.
   */
  const TagMerger& getDefault();

  const TagMerger& getMerger(const QString& name);

  /**
   * A convenience function for merging tags using the default mechanism. Equivalent to:
   * TagMergerFactory::getInstance().getDefault().mergeTags(t1, t2, et);
   */
  static Tags mergeTags(const Tags& t1, const Tags& t2, ElementType et);

private:
  QHash<QString, const TagMerger*> _mergers;
  const TagMerger* _default;

  static TagMergerFactory _theInstance;
};

}

#endif // TAGMERGERFACTORY_H
