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
#include <hoot/core/util/Configurable.h>
#include <hoot/core/elements/ElementType.h>

#include "ElementOsmMapVisitor.h"

namespace hoot
{

/**
 * Sets any tags on any elements with the specified key to the specified value
 */
class SetTagValueVisitor : public ElementOsmMapVisitor, public Configurable
{
public:

  static std::string className() { return "hoot::SetTagValueVisitor"; }

  SetTagValueVisitor();
  SetTagValueVisitor(QString key, QString value, bool appendToExistingValue = false,
                     //using Unknown as the default empty value for element type
                     ElementType elementType = ElementType::Unknown,
                     bool overwriteExistingTag = true);

  virtual void setConfiguration(const Settings& conf);

  virtual void visit(const boost::shared_ptr<Element>& e);

  virtual QString getDescription() const
  { return "Sets tags with the specified key to the specified value"; }

private:

  QStringList _k, _v;
  //if true; will not overwrite existing keys and will append values to them
  bool _appendToExistingValue;
  //an element type filter
  ElementType _elementType;
  //overwrites any tag with a matching key
  bool _overwriteExistingTag;

  void _setTag(const ElementPtr& e, QString k, QString v);
};

}

#endif // SETTAGVALUEVISITOR_H
