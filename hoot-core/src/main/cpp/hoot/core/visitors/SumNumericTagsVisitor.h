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
#ifndef SUM_NUMERIC_TAGS_VISITOR_H
#define SUM_NUMERIC_TAGS_VISITOR_H

// hoot
#include <hoot/core/info/SingleStatistic.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/visitors/ConstElementVisitor.h>

namespace hoot
{

/**
 * Sums numeric tag values with a specified key
 *
 * In the future, we may want to have this support substrings as well.
 */
class SumNumericTagsVisitor : public ConstElementVisitor, public SingleStatistic,
  public Configurable
{
public:

  static QString className() { return "SumNumericTagsVisitor"; }

  /**
   * @brief SumNumericTagsVisitor - Constructor
   */
  SumNumericTagsVisitor();
  /**
   * @brief SumNumericTagsVisitor - Constructor
   * @param keys keys of the tags whose values are to be summed
   */
  explicit SumNumericTagsVisitor(const QStringList keys);
  ~SumNumericTagsVisitor() override = default;

  /**
   * Given a set of tag keys and for all features having those tags, sums the numerical values of
   * the tags.  If the tag value cannot be converted to a number, a warning is logged and the tag
   * is skipped.
   *
   * @param e element to check for tag on
   */
  void visit(const ConstElementPtr& e) override;

  double getStat() const override { return _sum; }

  void setConfiguration(const Settings& conf) override;

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Sums numeric tag values with specified keys"; }

  QString getInitStatusMessage() const override { return "Summing values of numeric tags..."; }
  QString getCompletedStatusMessage() const override
  {
    return QString("Summed %1 numeric tags on %2 features.")
            .arg(StringUtils::formatLargeNumber(_numAffected), StringUtils::formatLargeNumber(_numProcessed));
  }

private:

  QStringList _keys;
  double _sum;
};

}

#endif // SUM_NUMERIC_TAGS_VISITOR_H
