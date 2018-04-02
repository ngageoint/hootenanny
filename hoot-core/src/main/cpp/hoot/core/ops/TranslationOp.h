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
#ifndef TRANSLATIONOP_H
#define TRANSLATIONOP_H

#include "OsmMapOperation.h"

// hoot
#include <hoot/core/util/Configurable.h>
#include <hoot/core/visitors/TranslationVisitor.h>

// Qt
#include <QString>

namespace hoot
{
class ScriptTranslator;

/**
 * Applies a translation to the given map.
 */
class TranslationOp : public OsmMapOperation, public Configurable
{
public:

  static std::string className() { return "hoot::TranslationOp"; }

  TranslationOp();

  virtual ~TranslationOp() {}

  virtual void apply(boost::shared_ptr<OsmMap>& map);

  void setConfiguration(const Settings& conf);

  /**
   * Set the path to the translation script.
   */
  void setPath(QString path) { _translator.setPath(path); }

  virtual QString getDescription() const { return "Translates features in a map"; }

private:

  TranslationVisitor _translator;
  bool _toOgr;
};

}

#endif // TRANSLATIONOP_H
