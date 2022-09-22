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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef TAGKEYCOUNTVISITOR_H
#define TAGKEYCOUNTVISITOR_H

// hoot
#include <hoot/core/info/SingleStatistic.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/visitors/ConstElementVisitor.h>

namespace hoot
{

/**
 * Counts the number of tags in all elements with the given keys
 */
class TagKeyCountVisitor : public ConstElementVisitor, public SingleStatistic, public Configurable
{
public:

  static QString className() { return "TagKeyCountVisitor"; }

  /**
   * @brief TagKeyCountVisitor - Constructor
   */
  TagKeyCountVisitor();
  /**
   * @brief TagKeyCountVisitor - Constructor
   * @param key single key to count
   */
  TagKeyCountVisitor(const QString& key);
  ~TagKeyCountVisitor() override = default;

  /**
   * @see ElementVisitor
   */
  void visit(const ConstElementPtr& e) override;

  double getStat() const override { return _keyCount; }

  /**
   * @see Configurable
   */
  void setConfiguration(const Settings& conf) override;

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override
  { return "Counts the number of tags with the given keys"; }

private:

  QStringList _keys;
  long _keyCount;
};

}

#endif // TAGKEYCOUNTVISITOR_H
