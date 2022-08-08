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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef TAGCONTAINSCRITERION_H
#define TAGCONTAINSCRITERION_H

// hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * Identifies elements by tag key and tag value substring
 */
class TagContainsCriterion : public ElementCriterion, public Configurable
{
public:

  static QString className() { return "TagContainsCriterion"; }

  TagContainsCriterion();
  TagContainsCriterion(QString key, QString valueSubstring);
  ~TagContainsCriterion() override = default;

 /**
  * Adds an additional pair to the search list. If any one of the pairs matches then it is
  * considered a match.
  */
  void addPair(QString key, QString valueSubstring);

  bool isSatisfied(const ConstElementPtr& e) const override;
  ElementCriterionPtr clone() override { return std::make_shared<TagContainsCriterion>(); }

  void setConfiguration(const Settings& s) override;

  QString toString() const override;
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override
  { return "Identifies elements having a particular tag key and tag value substring"; }

  void setKvps(const QStringList kvps);
  void setCaseSensitive(bool caseSens) { _caseSensitive = caseSens; }

private:

  QStringList _keys;
  QStringList _valueSubstrings;
  bool _caseSensitive;
};

}

#endif // TAGCONTAINSCRITERION_H
