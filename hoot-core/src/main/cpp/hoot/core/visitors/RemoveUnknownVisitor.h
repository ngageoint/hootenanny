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
#ifndef REMOVE_UNKNOWN_VISITOR_H
#define REMOVE_UNKNOWN_VISITOR_H

// hoot
#include <hoot/core/visitors/ElementOsmMapVisitor.h>

namespace hoot
{

/**
 * Removes all elements of a given status.
 */
class RemoveUnknownVisitor : public ElementOsmMapVisitor
{
public:

  static std::string className() { return "hoot::RemoveUnknownVisitor"; }

  void set(Status status) { _status = status; }

  virtual void visit(const std::shared_ptr<Element>& e) override;

  virtual QString getDescription() const
  { return "Removes all elements of Unknown1 or Unknown2 status"; }

  virtual QString getInitStatusMessage() const { return "Removing elements..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Removed " + QString::number(_numAffected) + " elements."; }

protected:

  RemoveUnknownVisitor(Status status) : _status(status) {}

private:

  RemoveUnknownVisitor() {}

  Status _status;
};

/**
 * Removes all elements that have a status of Unknown1.
 */
class RemoveUnknown1Visitor : public RemoveUnknownVisitor
{
public:

  static std::string className() { return "hoot::RemoveUnknown1Visitor"; }

  RemoveUnknown1Visitor() : RemoveUnknownVisitor(Status::Unknown1) {}

  virtual QString getDescription() const override
  { return "Removes all elements with a status of Unknown1"; }

  virtual std::string getClassName() const { return className(); }
};

/**
 * Removes all elements that have a status of Unknown2.
 */
class RemoveUnknown2Visitor : public RemoveUnknownVisitor
{
public:

  static std::string className() { return "hoot::RemoveUnknown2Visitor"; }

  RemoveUnknown2Visitor() : RemoveUnknownVisitor(Status::Unknown2) {}

  virtual QString getDescription() const override
  { return "Removes all elements with a status of Unknown2"; }

  virtual std::string getClassName() const { return className(); }
};

}

#endif // REMOVE_UNKNOWN_VISITOR_H
