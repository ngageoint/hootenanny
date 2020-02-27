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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef DUPLICATEREVIEWSOP_H
#define DUPLICATEREVIEWSOP_H

// Hoot
#include <hoot/core/io/Serializable.h>
#include <hoot/core/ops/OsmMapOperation.h>

// Standard
#include <set>

namespace hoot
{
class OsmMap;

/**
 * Goes through all relations and check if there are any duplicate reviews.
 */
class RemoveDuplicateReviewsOp : public OsmMapOperation, public Serializable
{
public:

  static std::string className() { return "hoot::RemoveDuplicateReviewsOp"; }

  RemoveDuplicateReviewsOp();

  virtual void apply(std::shared_ptr<OsmMap>& map) override;

  virtual std::string getClassName() const { return className(); }

  virtual void readObject(QDataStream& /*is*/) {}

  virtual void writeObject(QDataStream& /*os*/) const {}

  virtual QString getInitStatusMessage() const
  { return "Removing duplicate conflation review relations..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Removed " + QString::number(_numAffected) + " duplicate conflation review relations"; }

  virtual QString getDescription() const override { return "Removes duplicate conflation reviews"; }

private:

  std::shared_ptr<OsmMap> _map;

  int _taskStatusUpdateInterval;
};

}

#endif // DUPLICATEREVIEWSOP_H
