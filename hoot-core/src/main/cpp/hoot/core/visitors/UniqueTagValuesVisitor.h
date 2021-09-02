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
#ifndef UNIQUE_TAG_VALUES_VISITOR_H
#define UNIQUE_TAG_VALUES_VISITOR_H

// hoot
#include <hoot/core/visitors/ConstElementVisitor.h>

namespace hoot
{

/**
 * Collects unique tag values for a specified key
 */
class UniqueTagValuesVisitor : public ConstElementVisitor
{
public:

  static QString className() { return "UniqueTagValuesVisitor"; }

  /**
   * Constructor
   *
   * @param key tag key for which to collect for tag values
   * @param bag stores the unique tag values
   * @param split If split is set to true then the values in the tag are split before they're placed
   * in the bag.
   */
  UniqueTagValuesVisitor(QString key, std::set<QString>& bag, bool split = false);
  ~UniqueTagValuesVisitor() = default;

  /**
   * Puts all values for the given key into a bag. If you want to filter based on type see
   * FilteredVisitor. If the values are a list, then they're split before they're put in the bag.
   *
   * @see ElementVisitor
   */
  void visit(const ConstElementPtr& e) override;

  QString getDescription() const override { return "Collects unique tag values for a given key"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

private:

  QString _key;
  std::set<QString>& _bag;
  bool _split;
};

}

#endif // UNIQUE_TAG_VALUES_VISITOR_H
