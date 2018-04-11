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
 * @copyright Copyright (C) 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef NAMEDOP_H
#define NAMEDOP_H

// hoot
#include <hoot/core/util/Configurable.h>

#include "OsmMapOperation.h"

// Qt
#include <QStringList>

namespace hoot
{

/**
 * Applies a list of named operations to the given map. The named operations must implement either
 * OsmMapOperation or ConstElementVisitor and must be registered with the factory.
 */
class NamedOp : public OsmMapOperation, public Configurable
{
public:

  static std::string className() { return "hoot::NamedOp"; }

  NamedOp();
  NamedOp(QStringList namedOps);

  virtual ~NamedOp() {}

  virtual void apply(boost::shared_ptr<OsmMap>& map);

  virtual void setConfiguration(const Settings& conf);

  virtual QString getDescription() const { return ""; }

private:

  const Settings* _conf;
  QStringList _namedOps;
};

}

#endif // NAMEDOP_H
