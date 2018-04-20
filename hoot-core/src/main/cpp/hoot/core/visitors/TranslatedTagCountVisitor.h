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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef TRANSLATEDTAGCOUNTVISITOR_H
#define TRANSLATEDTAGCOUNTVISITOR_H

// hoot
#include <hoot/core/ConstOsmMapConsumer.h>
#include <hoot/core/elements/ConstElementVisitor.h>

#include "SingleStatistic.h"

namespace hoot
{
class Feature;
class ScriptToOgrTranslator;
class ScriptTranslator;
class Schema;

class TranslatedTagCountVisitor : public ConstElementVisitor, public ConstOsmMapConsumer,
  public SingleStatistic
{
public:

  static std::string className() { return "hoot::TranslatedTagCountVisitor"; }

  TranslatedTagCountVisitor() {}
  TranslatedTagCountVisitor(boost::shared_ptr<ScriptTranslator> t);

  virtual ~TranslatedTagCountVisitor() {}

  long getPopulatedCount() const { return _populatedCount; }

  long getDefaultCount() const { return _defaultCount; }

  long getNullCount() const { return _nullCount; }

  long getTotalCount() const { return getPopulatedCount() + getDefaultCount() + getNullCount(); }

  double getStat() const { return (double)getPopulatedCount() /
        (double)getTotalCount(); }

  virtual void setOsmMap(const OsmMap* map) { _map = map; }

  virtual void visit(const ConstElementPtr& e);

  virtual QString getDescription() const
  { return "Counts the number of tags that were able to be translated"; }

private:

  const OsmMap* _map;
  boost::shared_ptr<const Schema> _schema;
  boost::shared_ptr<ScriptToOgrTranslator> _translator;
  long _populatedCount, _defaultCount, _nullCount;

  void _countTags(boost::shared_ptr<Feature>& f);
};

}

#endif // TRANSLATEDTAGCOUNTVISITOR_H
