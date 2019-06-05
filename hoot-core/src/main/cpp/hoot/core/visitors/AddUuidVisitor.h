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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef ADDUUIDVISITOR_H
#define ADDUUIDVISITOR_H

// hoot
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * Adds a UUID with a specific key. If you want to limit the features that will be tagged then look
 * into FilteredVisitor.
 *
 * This really should be a ElementVisitor. See #2831.
 */
class AddUuidVisitor : public ConstElementVisitor, public ConstOsmMapConsumer, public Configurable
{
public:

  static std::string className() { return "hoot::AddUuidVisitor"; }

  AddUuidVisitor();
  AddUuidVisitor(QString key);

  virtual void setOsmMap(OsmMap* map) { _map = map; }

  /**
   * AddUuidVisitor requires a read/write map.
   */
  virtual void setOsmMap(const OsmMap* /*map*/) { assert(false); }

  /**
   * Adds a tag with a UUID to all valid elements.
   */
  virtual void visit(const ConstElementPtr& e);

  virtual QString getDescription() const { return "Adds unique ID tags"; }

  virtual void setConfiguration(const Settings& conf);

private:

  OsmMap* _map;
  QString _key;
};

}

#endif // ADDUUIDVISITOR_H
