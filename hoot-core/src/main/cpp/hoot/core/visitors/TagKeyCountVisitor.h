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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef TAGKEYCOUNTVISITOR_H
#define TAGKEYCOUNTVISITOR_H

// hoot
#include <hoot/core/elements/ElementVisitor.h>

#include "SingleStatistic.h"

namespace hoot
{

/**
 * Counts the number of tags in all elements with the given key
 */
class TagKeyCountVisitor : public ElementVisitor, public SingleStatistic
{
  public:

    static string className() { return "hoot::TagKeyCountVisitor"; }

    TagKeyCountVisitor() {}
    TagKeyCountVisitor(const QString key);

    virtual ~TagKeyCountVisitor() {}

    virtual void visit(const ConstElementPtr& e);

    virtual double getStat() const { return _keyCount; }

  private:

    QString _key;
    long _keyCount;

};

}

#endif // TAGKEYCOUNTVISITOR_H
