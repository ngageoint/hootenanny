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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef ADDEXPORTTAGSVISITOR_H
#define ADDEXPORTTAGSVISITOR_H

#include <hoot/core/elements/ElementVisitor.h>

namespace hoot
{

/**
 * Historically the tags for export have been added in the OsmMapWriter code. This creates
 * duplicate code, is inconsistent and difficult to maintain/use. The intent of this visitor is
 * to centralize the creation of these tags. This comes at the cost of cloning every element during
 * the export/write operation (to avoid modifying the input element), but I think it is likely
 * worth the cost.
 *
 * @todo refactor all existing writers to use this visitor.
 */
class AddExportTagsVisitor : public ElementVisitor
{
public:

  AddExportTagsVisitor();

  static std::string className() { return "hoot::AddExportTagsVisitor"; }

  virtual void visit(const ElementPtr& pElement);

  virtual QString getDescription() const { return "Adds tags needed for exporting"; }

  void setIncludeHootInfo(bool includeInfo) { _includeDebug = includeInfo; }
  void setIncludeIds(bool includeIds) { _includeIds = includeIds; }
  void setIncludeCircularError(bool includeCircularError)
  { _includeCircularError = includeCircularError; }
  void overrideDebugSettings();

  virtual std::string getClassName() const { return className(); }

private:

  bool _includeIds;
  bool _textStatus;
  bool _includeCircularError;
  bool _includeDebug;

  // moved this out of Status.cpp, for local consumption only
  QString toCompatString(Status status) const;
};

}

#endif // ADDEXPORTTAGSVISITOR_H
