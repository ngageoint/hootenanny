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
#ifndef SPLITNAMEVISITOR_H
#define SPLITNAMEVISITOR_H

#include <hoot/core/visitors/ElementOsmMapVisitor.h>

namespace hoot
{

/**
 * Sometimes name fields have more than 255 characters. This method takes all names beyond the 255
 * character point and puts them into fields named 'name:0', 'name:1', etc.
 */
class SplitNameVisitor : public ElementOsmMapVisitor
{
public:

  static std::string className() { return "hoot::SplitNameVisitor"; }

  SplitNameVisitor();

  void setMaxSize(int s) { _maxSize = s; }

  virtual void visit(const boost::shared_ptr<Element>& e);

  virtual QString getDescription() const
  { return "Splits name tags over 255 characters"; }

private:

  int _maxSize;

  void _addExtraNames(Tags& t, const QStringList& extraNames);

  int _getNextNameId(const Tags& t, int lastId);

  /**
   * Given a name list greater than 255 characters, split the names up so that the result is a
   * string <= 255 characters and put all the extra names into extras.
   *
   * If a name is greater than 255 characters it is cropped down to 252 characters plus "..." at the
   * end.
   */
  QStringList _splitNames(const QString& v, QStringList& extras);
};

}

#endif // SPLITNAMEVISITOR_H
