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

  static std::string className() { return "hoot::TagCriterion"; }

  TagCriterion();
  TagCriterion(const QString& k, const QString& v);
  virtual ~TagCriterion() = default;

  virtual bool isSatisfied(const ConstElementPtr& e) const override;

  void setConfiguration(const Settings& s);

  void setKvps(const QStringList kvps);
  void setCaseSensitive(bool caseSens) { _caseSensitive = caseSens; }

  virtual ElementCriterionPtr clone() { return ElementCriterionPtr(new TagCriterion()); }

  virtual QString getDescription() const
  { return "Identifies elements containing a specified tag key/value combination"; }

  virtual QString toString() const override;

private:

  QStringList _kvps;
  bool _caseSensitive;
};

}

#endif // TAGCRITERION_H
