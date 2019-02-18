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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef TAGRENAMEKEYVISITOR_H
#define TAGRENAMEKEYVISITOR_H

// hoot
#include <hoot/core/util/Configurable.h>
#include <hoot/core/visitors/ElementOsmMapVisitor.h>

namespace hoot
{

/**
 * Renames all keys for all elements with the specified key to a new key
 */
class TagRenameKeyVisitor : public ElementOsmMapVisitor, public Configurable
{
public:

  static std::string className() { return "hoot::TagRenameKeyVisitor"; }

  TagRenameKeyVisitor();

  TagRenameKeyVisitor(const QString oldKey, const QString newKey);

  virtual void visit(const boost::shared_ptr<Element>& e) override;

  virtual void setConfiguration(const Settings& conf);

  virtual QString getDescription() const { return "Renames tag keys"; }

private:

  QString _oldKey;
  QString _newKey;
};

}

#endif // TAGRENAMEKEYVISITOR_H
