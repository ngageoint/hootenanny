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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef TAGCONTAINSCRITERION_H
#define TAGCONTAINSCRITERION_H

// hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/util/Configurable.h>

// Qt
#include <QStringList>

namespace hoot
{

/**
 * Identifies elements by tag key and tag value substring
 */
class TagContainsCriterion : public ElementCriterion, public Configurable
{
public:

  static std::string className() { return "hoot::TagContainsCriterion"; }

  TagContainsCriterion() {}
  TagContainsCriterion(QString key, QString valueSubstring);
  TagContainsCriterion(QStringList keys, QStringList valueSubstrings);

  virtual bool isSatisfied(const ConstElementPtr& e) const override;

  void setConfiguration(const Settings& s);

 /**
   * Adds an additional pair to the search list. If any one of the pairs matches then it is
   * considered a match.
   */
  void addPair(QString key, QString valueSubstring);

  virtual ElementCriterionPtr clone() { return ElementCriterionPtr(new TagContainsCriterion()); }

  virtual QString getDescription() const
  { return "Identifies elements having a particular tag key and tag value substring"; }

  void setKvps(const QStringList kvps);
  void setCaseSensitive(bool caseSens) { _caseSensitive = caseSens; }

  virtual QString toString() const override;

private:

  QStringList _keys;
  QStringList _valueSubstrings;
  bool _caseSensitive;
};

}

#endif // TAGCONTAINSCRITERION_H
