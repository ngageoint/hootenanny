///*
// * This file is part of Hootenanny.
// *
// * Hootenanny is free software: you can redistribute it and/or modify
// * it under the terms of the GNU General Public License as published by
// * the Free Software Foundation, either version 3 of the License, or
// * (at your option) any later version.
// *
// * This program is distributed in the hope that it will be useful,
// * but WITHOUT ANY WARRANTY; without even the implied warranty of
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// * GNU General Public License for more details.
// *
// * You should have received a copy of the GNU General Public License
// * along with this program.  If not, see <http://www.gnu.org/licenses/>.
// *
// * --------------------------------------------------------------------
// *
// * The following copyright notices are generated automatically. If you
// * have a new notice to add, please use the format:
// * " * @copyright Copyright ..."
// * This will properly maintain the copyright information. DigitalGlobe
// * copyrights will be updated automatically.
// *
// * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
// */
//#ifndef REMOVEREF2VISITORMULTIPLECRITERION_H
//#define REMOVEREF2VISITORMULTIPLECRITERION_H

//This is disabled for now.  May be needed later for area to area conflation.  See #1174.

//// hoot

//#include "RemoveRef2Visitor.h"

//namespace hoot
//{

///**
// * If one specified criterion meets for REF1 and another for REF2 elements, then the REF2 tag is
// * removed.
// *
// * This class is re-entrant, but not thread safe.
// */
//class RemoveRef2VisitorMultipleCriterion : public RemoveRef2Visitor
//{

//public:

//  static std::string className() { return "hoot::RemoveRef2VisitorMultipleCriterion"; }

//  RemoveRef2VisitorMultipleCriterion();
//  virtual ~RemoveRef2VisitorMultipleCriterion() {}

//  virtual void addCriterion(const ElementCriterionPtr& e);

//  virtual void visit(const ConstElementPtr& e);

//  virtual bool ref1CriterionSatisfied(const ConstElementPtr& e) const;
//  virtual bool ref2CriterionSatisfied(const ConstElementPtr& e) const;

//private:

//  ElementCriterionPtr _ref1Criterion;
//  ElementCriterionPtr _ref2Criterion;

//};

//}

//#endif // REMOVEREF2VISITORMULTIPLECRITERION_H
