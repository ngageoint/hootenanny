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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef TAGKEYCRITERION_H
#define TAGKEYCRITERION_H

// Hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/util/Configurable.h>

// Qt
#include <QStringList>

namespace hoot
{

/**
 * Identifies elements that contain a specified tag key
 */
class TagKeyCriterion : public ElementCriterion, public Configurable
{
public:

  static std::string className() { return "hoot::TagKeyCriterion"; }

  TagKeyCriterion() = default;
  explicit TagKeyCriterion(QString key);
  TagKeyCriterion(QString key1, QString key2);
  TagKeyCriterion(QString key1, QString key2, QString key3);
  virtual ~TagKeyCriterion() = default;

  void addKey(QString key);

  virtual bool isSatisfied(const ConstElementPtr& e) const override;

  virtual ElementCriterionPtr clone() { return ElementCriterionPtr(new TagKeyCriterion(_keys)); }

  virtual QString getDescription() const
  { return "Identifies elements that contain a specified tag key"; }

  virtual void setConfiguration(const Settings& conf);

  virtual QString toString() const override
  { return QString::fromStdString(className()).remove("hoot::"); }

protected:

  explicit TagKeyCriterion(QStringList keys);

private:

  QStringList _keys;
};

}

#endif // TAGKEYCRITERION_H
