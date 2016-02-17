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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef TRANSLATIONVISITOR_H
#define TRANSLATIONVISITOR_H

// Boost
#include <boost/shared_ptr.hpp>

// Hoot
#include <hoot/core/ConstOsmMapConsumer.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/io/ScriptToOgrTranslator.h>
#include <hoot/core/io/ScriptTranslator.h>

// Qt
#include <QString>

namespace hoot
{

class TranslationVisitor : public ElementVisitor, public ConstOsmMapConsumer
{
public:

  TranslationVisitor(ScriptTranslator& t, bool toOgr, OsmMap* map);
  virtual ~TranslationVisitor() {}

  virtual void setOsmMap(OsmMap* map) { _map = map; }

  virtual void setOsmMap(const OsmMap*) { assert(false); }

  virtual void visit(const ConstElementPtr& ce);

private:
  ScriptTranslator& _t;
  ScriptToOgrTranslator* _togr;
  QString _circularErrorKey;
  QString _accuracyKey;
  OsmMap* _map;
  bool _toOgr;
};

}

#endif // TRANSLATIONVISITOR_H
