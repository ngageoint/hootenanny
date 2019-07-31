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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef RECURSIVE_SET_TAG_VALUE_OP_H
#define RECURSIVE_SET_TAG_VALUE_OP_H

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/info/OperationStatusInfo.h>
#include <hoot/core/criterion/ElementCriterion.h>

namespace hoot
{

/**
 * TODO
 */
class RecursiveSetTagValueOp : public OsmMapOperation, public OperationStatusInfo
{
public:

  static std::string className() { return "hoot::RecursiveSetTagValueOp"; }

  RecursiveSetTagValueOp();
  RecursiveSetTagValueOp(ElementCriterionPtr elementCriterion, const QString& key,
                         const QString& val);

  virtual void apply(std::shared_ptr<OsmMap>& map) override;

  virtual QString getDescription() const override { return "TODO"; }

  virtual QString getInitStatusMessage() const { return "TODO"; }

  virtual QString getCompletedStatusMessage() const { return "TODO"; }

private:

  ElementCriterionPtr _crit;
  QString _key;
  QString _val;
};

}

#endif // RECURSIVE_SET_TAG_VALUE_OP_H
