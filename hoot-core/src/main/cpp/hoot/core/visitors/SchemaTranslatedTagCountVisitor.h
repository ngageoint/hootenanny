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
#ifndef SCHEMATRANSLATEDTAGCOUNTVISITOR_H
#define SCHEMATRANSLATEDTAGCOUNTVISITOR_H

// hoot
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/visitors/ConstElementVisitor.h>
#include <hoot/core/info/SingleStatistic.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/schema/ScriptToOgrSchemaTranslator.h>

namespace hoot
{

class Feature;
class ScriptSchemaTranslator;
class Schema;

class SchemaTranslatedTagCountVisitor : public ConstElementVisitor, public ConstOsmMapConsumer,
  public SingleStatistic
{
public:

  static QString className() { return "SchemaTranslatedTagCountVisitor"; }

  SchemaTranslatedTagCountVisitor();
  SchemaTranslatedTagCountVisitor(const std::shared_ptr<ScriptSchemaTranslator>& t);
  ~SchemaTranslatedTagCountVisitor() = default;

  long getPopulatedCount() const { return _populatedCount; }
  long getDefaultCount() const { return _defaultCount; }
  long getNullCount() const { return _nullCount; }
  long getTotalCount() const { return getPopulatedCount() + getDefaultCount() + getNullCount(); }

  double getStat() const override { return (double)getPopulatedCount() / (double)getTotalCount(); }

  void setOsmMap(const OsmMap* map) override { _map = map; }

  void visit(const ConstElementPtr& e) override;

  QString getDescription() const override
  { return "Counts the number of tags translated to a schema"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

  QString getInitStatusMessage() const override { return "Counting translated tags..."; }
  QString getCompletedStatusMessage() const override
  {
    return
      "Counted " + StringUtils::formatLargeNumber(getTotalCount()) + " translated tags on " +
      StringUtils::formatLargeNumber(_numAffected) + " features.";
  }

  void setTranslator(const std::shared_ptr<ScriptSchemaTranslator>& translator);

private:

  const OsmMap* _map;
  std::shared_ptr<const Schema> _schema;
  std::shared_ptr<ScriptToOgrSchemaTranslator> _translator;
  long _populatedCount, _defaultCount, _nullCount;

  int _taskStatusUpdateInterval;

  void _countTags(const std::shared_ptr<Feature>& f);
};

}

#endif // SCHEMATRANSLATEDTAGCOUNTVISITOR_H
