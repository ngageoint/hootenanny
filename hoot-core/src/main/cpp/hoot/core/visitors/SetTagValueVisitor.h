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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef SETTAGVALUEVISITOR_H
#define SETTAGVALUEVISITOR_H

// hoot
#include <hoot/core/criterion/ElementCriterionConsumer.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/visitors/ElementOsmMapVisitor.h>
#include <hoot/core/info/OperationStatusInfo.h>

namespace hoot
{

/**
 * Sets tags on any elements with the specified key to the specified value or adds a new tag, if
 * the tag doesn't exist on the element.
 */
class SetTagValueVisitor : public ElementOsmMapVisitor, public Configurable,
  public ElementCriterionConsumer, public OperationStatusInfo
{
public:

  static std::string className() { return "hoot::SetTagValueVisitor"; }

  SetTagValueVisitor();
  SetTagValueVisitor(QString key, QString value, bool appendToExistingValue = false,
                     const QString criterionName = "", bool overwriteExistingTag = true,
                     bool negateCriterion = false);

  virtual void addCriterion(const ElementCriterionPtr& e);

  virtual void setConfiguration(const Settings& conf);

  virtual void visit(const boost::shared_ptr<Element>& e);

  virtual QString getDescription() const
  { return "Adds or updates one or more tags with a specified key/value combination"; }

  void setNegateCriterion(bool negate) { _negateCriterion = negate; }

  virtual QString getInitStatusMessage() const { return "Updating tags..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Updated " + QString::number(_numAffected) + " tags"; }

private:

  QStringList _k, _v;
  //if true; will not overwrite existing keys and will append values to them
  bool _appendToExistingValue;
  //a customizable filter
  boost::shared_ptr<ElementCriterion> _criterion;
  //overwrites any tag with a matching key
  bool _overwriteExistingTag;
  //This allows for negating the criterion as an option sent in from the command line.
  bool _negateCriterion;

  void _setTag(const ElementPtr& e, QString k, QString v);
  void _setCriterion(const QString criterionName);
};

}

#endif // SETTAGVALUEVISITOR_H
