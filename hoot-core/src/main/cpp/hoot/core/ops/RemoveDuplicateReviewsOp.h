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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef DUPLICATEREVIEWSOP_H
#define DUPLICATEREVIEWSOP_H

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/io/Serializable.h>
#include <hoot/core/info/OperationStatusInfo.h>

// Standard
#include <set>

namespace hoot
{
class OsmMap;

/**
 * Goes through all relations and check if there are any duplicate reviews.
 */
class RemoveDuplicateReviewsOp : public OsmMapOperation, public Serializable,
  public OperationStatusInfo
{
public:

  static std::string className() { return "hoot::RemoveDuplicateReviewsOp"; }

  RemoveDuplicateReviewsOp();

  virtual void apply(boost::shared_ptr<OsmMap>& map) override;

  virtual std::string getClassName() const { return className(); }

  virtual void readObject(QDataStream& /*is*/) {}

  virtual void writeObject(QDataStream& /*os*/) const {}

  virtual QString getInitStatusMessage() const { return "Removing duplicate review relations..."; }

  // finish; wasn't obvious how to count the total affected - #2933
  virtual QString getCompletedStatusMessage() const
  { return ""; }

  virtual QString getDescription() const override { return "Removes duplicate reviews"; }

private:

  boost::shared_ptr<OsmMap> _map;
};

}

#endif // DUPLICATEREVIEWSOP_H
