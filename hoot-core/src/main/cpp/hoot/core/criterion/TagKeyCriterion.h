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
#ifndef TAGKEYCRITERION_H
#define TAGKEYCRITERION_H

// Hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * Identifies elements that contain a specified tag key
 */
class TagKeyCriterion : public ElementCriterion, public Configurable
{
public:

  static QString className() { return "TagKeyCriterion"; }

  TagKeyCriterion() = default;
  explicit TagKeyCriterion(QString key);
  explicit TagKeyCriterion(QStringList keys);
  TagKeyCriterion(QString key1, QString key2);
  TagKeyCriterion(QString key1, QString key2, QString key3);
  ~TagKeyCriterion() override = default;

  void addKey(QString key);

  bool isSatisfied(const ConstElementPtr& e) const override;
  ElementCriterionPtr clone() override { return std::make_shared<TagKeyCriterion>(_keys); }

  void setConfiguration(const Settings& conf) override;

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString toString() const override { return className(); }
  QString getDescription() const override
  { return "Identifies elements that contain a specified tag key"; }

private:

  QStringList _keys;
};

}

#endif // TAGKEYCRITERION_H
