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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef SCHEMATRANSLATEDTAGCOUNTVISITOR_H
#define SCHEMATRANSLATEDTAGCOUNTVISITOR_H

// hoot
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/info/SingleStatistic.h>
#include <hoot/core/util/StringUtils.h>

namespace hoot
{

class Feature;
class ScriptToOgrSchemaTranslator;
class ScriptSchemaTranslator;
class Schema;

class SchemaTranslatedTagCountVisitor : public ConstElementVisitor, public ConstOsmMapConsumer,
  public SingleStatistic
{
public:

  static std::string className() { return "hoot::SchemaTranslatedTagCountVisitor"; }

  SchemaTranslatedTagCountVisitor() {}
  SchemaTranslatedTagCountVisitor(const std::shared_ptr<ScriptSchemaTranslator>& t);

  long getPopulatedCount() const { return _populatedCount; }
  long getDefaultCount() const { return _defaultCount; }
  long getNullCount() const { return _nullCount; }
  long getTotalCount() const { return getPopulatedCount() + getDefaultCount() + getNullCount(); }

  double getStat() const { return (double)getPopulatedCount() / (double)getTotalCount(); }

  virtual void setOsmMap(const OsmMap* map) { _map = map; }

  virtual void visit(const ConstElementPtr& e);

  virtual QString getDescription() const
  { return "Counts the number of tags translated to a schema"; }

  virtual std::string getClassName() const { return className(); }

  virtual QString getInitStatusMessage() const { return "Counting translated tags..."; }

  virtual QString getCompletedStatusMessage() const
  {
    return
      "Counted " + StringUtils::formatLargeNumber(getTotalCount()) + " translated tags on " +
      StringUtils::formatLargeNumber(_numAffected) + " features.";
  }

private:

  const OsmMap* _map;
  std::shared_ptr<const Schema> _schema;
  std::shared_ptr<ScriptToOgrSchemaTranslator> _translator;
  long _populatedCount, _defaultCount, _nullCount;

  int _taskStatusUpdateInterval;

  void _countTags(std::shared_ptr<Feature>& f);
};

}

#endif // SCHEMATRANSLATEDTAGCOUNTVISITOR_H
