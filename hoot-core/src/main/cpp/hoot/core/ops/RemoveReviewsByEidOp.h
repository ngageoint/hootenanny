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
#ifndef REMOVEREVIEWSBYEIDOP_H
#define REMOVEREVIEWSBYEIDOP_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/ConstElementConsumer.h>

// Standard
#include <set>

#include "ConstOsmMapOperation.h"

namespace hoot
{
using namespace std;

/**
 *
 */
class RemoveReviewsByEidOp : public ConstOsmMapOperation, public ConstElementConsumer
{
public:
  static string className() { return "hoot::RemoveReviewsByEidOp"; }

  /**
   */
  RemoveReviewsByEidOp(ElementId eid, bool clearAndRemoveElement = false);

  /**
   * It is expected that the eid will be populated with addElement after construction. addElement
   * must be called exactly two times.
   */
  RemoveReviewsByEidOp() {}

  virtual ~RemoveReviewsByEidOp() {}

  /**
   * If the elements aren't specified in the constructor this must be called exactly two times. Once
   * for 'from' and a second time for 'to'.
   */
  virtual void addElement(const ConstElementPtr& e);

  virtual void apply(const shared_ptr<OsmMap>& map);

private:
  ElementId _eid;
  bool _clearAndRemove;
};

}

#endif // REMOVEREVIEWSBYEIDOP_H
