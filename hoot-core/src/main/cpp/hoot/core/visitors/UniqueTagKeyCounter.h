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

#ifndef UNIQUE_TAG_KEY_COUNTER_H
#define UNIQUE_TAG_KEY_COUNTER_H

// hoot
#include <hoot/core/visitors/ConstElementVisitor.h>
#include <hoot/core/info/SingleStatistic.h>

namespace hoot
{

/**
 * Counts unique instances of tag keys in elements
 */
class UniqueTagKeyCounter : public ConstElementVisitor, public SingleStatistic
{
public:

  static QString className() { return "hoot::UniqueTagKeyCounter"; }

  UniqueTagKeyCounter() = default;
  ~UniqueTagKeyCounter() = default;

  /**
   * @see ConstElementVisitor
   */
  void visit(const ConstElementPtr& e) override;

  /**
   * @see SingleStatistic
   */
  double getStat() const override { return (double)_tagKeys.size(); }

  QString getDescription() const override { return "Counts the number of unique tag keys"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

private:

  std::set<QString> _tagKeys;
};

}

#endif // UNIQUE_TAG_KEY_COUNTER_H
