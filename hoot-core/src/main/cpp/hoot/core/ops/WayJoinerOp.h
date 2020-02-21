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
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef __WAY_JOINER_OP_H__
#define __WAY_JOINER_OP_H__

//  Hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/algorithms/WayJoiner.h>

namespace hoot
{

class WayJoinerOp : public OsmMapOperation, public Configurable
{
public:

  WayJoinerOp();

  static std::string className() { return "hoot::WayJoinerOp"; }

  /**
   * Apply the way joiner to the specified map
   */
  virtual void apply(std::shared_ptr<OsmMap>& map);

  virtual void setConfiguration(const Settings& conf);

  virtual QString getDescription() const
  { return "Joins ways split during cleaning and conflation matching operations"; }

  virtual QString getInitStatusMessage() const
  { return "Rejoining ways split during conflation..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Rejoined " + QString::number(_numAffected) + " ways"; }

  /**
   * @see FilteredByCriteria
   */
  virtual QStringList getCriteria() const;

  virtual std::string getClassName() const { return className(); }

private:

  std::shared_ptr<WayJoiner> _wayJoiner;
};

}

#endif  //  __WAY_JOINER_OP_H__
