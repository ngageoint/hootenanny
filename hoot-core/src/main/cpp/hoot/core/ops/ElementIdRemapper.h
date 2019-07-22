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

#ifndef ELEMENT_ID_REMAPPER_H
#define ELEMENT_ID_REMAPPER_H

#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/info/OperationStatusInfo.h>
#include <hoot/core/ops/OsmMapOperation.h>

namespace hoot
{

class ElementIdRemapper : public OsmMapOperation, public OperationStatusInfo
{
public:

  static std::string className() { return "hoot::ElementIdRemapper"; }

  ElementIdRemapper();

  virtual void apply(OsmMapPtr& map) override;

  void setRemapNodes(bool remap) { _remapNodes = remap; }
  void setRemapWays(bool remap) { _remapWays = remap; }
  void setRemapRelations(bool remap) { _remapRelations = remap; }
  void setIdGen(const std::shared_ptr<IdGenerator>& idGen) { _idGen = idGen; }

  virtual QString getDescription() const override
  { return "TODO"; }

  virtual QString getInitStatusMessage() const override
  { return "TODO."; }

  virtual QString getCompletedStatusMessage() const override
  { return "TODO"; }

private:

  bool _remapNodes;
  bool _remapWays;
  bool _remapRelations;
  std::shared_ptr<IdGenerator> _idGen;
};

}

#endif  //  ELEMENT_ID_REMAPPER_H
