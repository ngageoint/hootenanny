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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef REMOVEREF2VISITOR_H
#define REMOVEREF2VISITOR_H

// hoot
#include <hoot/core/OsmMapConsumer.h>
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/filters/ElementCriterionConsumer.h>

// Qt
#include <QMap>
#include <QMutex>

// tgs
#include <tgs/SharedPtr.h>

namespace hoot
{

/**
 * If the specified criterion meets for both the REF1 and REF2 elements, then the REF2 tag is
 * removed.
 *
 * This class is re-entrant, but not thread safe.
 */
class RemoveRef2Visitor :
    public ElementVisitor,
    public OsmMapConsumer,
    public ElementCriterionConsumer
{
public:
  typedef QMap<QString, ElementId> Ref1ToEid;

  static std::string className() { return "hoot::RemoveRef2Visitor"; }

  RemoveRef2Visitor();
  virtual ~RemoveRef2Visitor() {}

  virtual void addCriterion(const ElementCriterionPtr& e);

  virtual void setOsmMap(OsmMap* map);
  virtual void setOsmMap(const OsmMap* /*map*/) { assert(false); }

  virtual void visit(const ConstElementPtr& e);

private:
  OsmMap* _map;
  ElementCriterionPtr _criterion;
  Ref1ToEid _ref1ToEid;
  static QStringList _ref2Keys;
  static QMutex _mutex;

  void _checkAndDeleteRef2(ElementPtr e, QString ref);

  bool _hasRef2Tag(ElementPtr e) const;

};

}

#endif // REMOVEREF2VISITOR_H
