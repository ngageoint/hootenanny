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
 * @copyright Copyright (C) 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef REMOVETAGSVISITOR_H
#define REMOVETAGSVISITOR_H

// hoot
#include <hoot/core/criterion/ElementCriterionConsumer.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/visitors/ElementOsmMapVisitor.h>

namespace hoot
{

/**
 * Removes any tags with keys matching those passed to this visitor
 */
class RemoveTagsVisitor : public ElementOsmMapVisitor, public Configurable,
  public ElementCriterionConsumer
{
public:

  static std::string className() { return "hoot::RemoveTagsVisitor"; }

  RemoveTagsVisitor();
  explicit RemoveTagsVisitor(QString key);
  RemoveTagsVisitor(QString key1, QString key2);
  explicit RemoveTagsVisitor(QStringList keys);

  virtual void addCriterion(const ElementCriterionPtr& e);

  void setConfiguration(const Settings& conf);

  void addKey(QString key);

  virtual void visit(const boost::shared_ptr<Element>& e);

  virtual QString getDescription() const { return "Removes tags with matching keys"; }

  void setNegateCriterion(bool negate) { _negateCriterion = negate; }

private:

  QStringList _keys;
  boost::shared_ptr<ElementCriterion> _criterion;
  //This allows for negating the criterion as an option sent in from the command line.
  bool _negateCriterion;

  void _setCriterion(const QString criterionName);
};

}


#endif // REMOVETAGSVISITOR_H
