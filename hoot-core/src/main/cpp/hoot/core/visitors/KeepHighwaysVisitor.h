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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef KEEPHIGHWAYSVISITOR_H
#define KEEPHIGHWAYSVISITOR_H

// hoot
#include <hoot/core/visitors/ConstElementVisitor.h>
#include <hoot/core/elements/OsmMapConsumer.h>

namespace hoot
{

/**
 * Removes all ways and relations that are not part of a linear highway.
 * Note: You may have to run this multiple times to get the desired effect.
 *
 * @todo delete
 */
class KeepHighwaysVisitor : public ConstElementVisitor, public OsmMapConsumer
{
public:

  static QString className() { return "hoot::KeepHighwaysVisitor"; }

  KeepHighwaysVisitor() = default;
  ~KeepHighwaysVisitor() = default;

  void setOsmMap(OsmMap* map) override { _map = map; }
  /**
   * KeepHighwaysVisitor requires a read/write map.
   */
  void setOsmMap(const OsmMap* /*map*/) const { assert(false); }

  void visit(const ConstElementPtr& e) override;

  QString getDescription() const override
  { return "Removes all ways and relations that are not part of a linear highway"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

private:

  OsmMap* _map;
};

}

#endif // KEEPHIGHWAYSVISITOR_H
