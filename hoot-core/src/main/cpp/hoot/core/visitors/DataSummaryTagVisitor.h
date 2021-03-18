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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */
#ifndef DATA_SUMMARY_TAG_VISITOR_H
#define DATA_SUMMARY_TAG_VISITOR_H

// hoot
#include <hoot/core/util/Configurable.h>
#include <hoot/core/visitors/ElementVisitor.h>

namespace hoot
{

/**
 *  Add data summary tags for `first seen`, `last seen`, `number of observations`,
 *  `number of misses`, `max consecutive runs` for an element with a tag containing a list of
 *  dates observed. If you want to limit the features that will be tagged then look into
 *  FilteredVisitor.
 */
class DataSummaryTagVisitor : public ElementVisitor, public Configurable
{
public:

  static QString className() { return "hoot::DataSummaryTagVisitor"; }

  DataSummaryTagVisitor();
  DataSummaryTagVisitor(const QString& sources);
  virtual ~DataSummaryTagVisitor() = default;

  /**
   * Adds a tag with a UUID to all valid elements.
   */
  virtual void visit(const ElementPtr& e);

  virtual QString getDescription() const
  { return "Adds summary tags to conflated datasets for further analysis"; }

  virtual void setConfiguration(const Settings& conf);

  virtual QString getName() const { return className(); }

  virtual QString getClassName() const override { return className(); }

private:

  QStringList _sources;
};

}

#endif // DATA_SUMMARY_TAG_VISITOR_H
