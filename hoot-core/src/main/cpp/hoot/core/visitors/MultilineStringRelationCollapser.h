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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef MULTILINESTRING_RELATION_COLLAPSER_H
#define MULTILINESTRING_RELATION_COLLAPSER_H

// Hoot
#include <hoot/core/elements/OsmMapConsumer.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/visitors/MultipleCriterionConsumerVisitor.h>

namespace hoot
{

/**
 * TODO
 */
class MultilineStringRelationCollapser : public MultipleCriterionConsumerVisitor,
  public OsmMapConsumer, public Configurable
{
public:

  static std::string className() { return "hoot::MultilineStringRelationCollapser"; }

  MultilineStringRelationCollapser();
  virtual ~MultilineStringRelationCollapser() = default;

  /**
   * @see ElementVisitor
   */
  virtual void visit(const ElementPtr& e);

  /**
   * @see OsmMapConsumer
   */
  virtual void setOsmMap(OsmMap* map) { _map = map->shared_from_this(); }

  virtual QString getInitStatusMessage() const { return "TODO..."; }

  virtual QString getCompletedStatusMessage() const
  { return "TODO"; }

  virtual QString getDescription() const { return "TODO"; }

  virtual std::string getClassName() const { return className(); }

  /**
   * @see Configurable
   */
  virtual void setConfiguration(const Settings& conf);

private:

  OsmMapPtr _map;

  // TODO
  QStringList _typeKeys;
  QStringList _typeKvps;

  // TODO
  int _numRelationMembersModified;
};

}

#endif // MULTILINESTRING_RELATION_COLLAPSER_H
