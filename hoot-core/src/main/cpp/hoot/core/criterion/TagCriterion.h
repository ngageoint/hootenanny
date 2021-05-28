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
#ifndef TAGCRITERION_H
#define TAGCRITERION_H

// hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/util/Configurable.h>

// Qt
#include <QString>

namespace hoot
{

/**
 * Returns true if k==v
 */
class TagCriterion : public ElementCriterion, public Configurable
{
public:

  static QString className() { return "hoot::TagCriterion"; }

  TagCriterion();
  TagCriterion(const QString& k, const QString& v);
  ~TagCriterion() = default;

  bool isSatisfied(const ConstElementPtr& e) const override;

  void setConfiguration(const Settings& s) override;

  void setKvps(const QStringList kvps);
  void setCaseSensitive(bool caseSens) { _caseSensitive = caseSens; }

  ElementCriterionPtr clone() override { return ElementCriterionPtr(new TagCriterion()); }

  QString getDescription() const override
  { return "Identifies elements containing a specified tag key/value combination"; }
  QString toString() const override;
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

private:

  QStringList _kvps;
  bool _caseSensitive;
};

}

#endif // TAGCRITERION_H
