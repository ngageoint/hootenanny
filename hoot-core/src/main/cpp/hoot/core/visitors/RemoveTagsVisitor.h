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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
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
 *
 * This really should be an ElementVisitor. See #2831.
 */
class RemoveTagsVisitor : public ElementOsmMapVisitor, public Configurable,
  public ElementCriterionConsumer
{
public:

  static std::string className() { return "hoot::RemoveTagsVisitor"; }

  RemoveTagsVisitor();
  explicit RemoveTagsVisitor(const QStringList& keys);

  virtual void addCriterion(const ElementCriterionPtr& e);

  void setConfiguration(const Settings& conf);

  virtual void visit(const std::shared_ptr<Element>& e);

  virtual QString getDescription() const { return "Removes tags by key"; }

  void setNegateCriterion(bool negate) { _negateCriterion = negate; }

  virtual QString getInitStatusMessage() const
  { return "Removing tags..."; }

  virtual QString getCompletedStatusMessage() const
  {
    return
      "Removed " + QString::number(_numTagsRemoved) + " tags from " +
      QString::number(_numAffected) + " different elements";
  }

  virtual std::string getClassName() const { return className(); }

protected:

  QStringList _keys;
  std::shared_ptr<ElementCriterion> _criterion;
  //This allows for negating the criterion as an option sent in from the command line.
  bool _negateCriterion;
  long _numTagsRemoved;

private:

  void _setCriterion(const QString& criterionName);
};

}

#endif // REMOVETAGSVISITOR_H
