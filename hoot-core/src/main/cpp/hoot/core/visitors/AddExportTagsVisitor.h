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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef ADDEXPORTTAGSVISITOR_H
#define ADDEXPORTTAGSVISITOR_H

#include <hoot/core/visitors/ElementVisitor.h>

namespace hoot
{

/**
 * Historically the tags for export have been added in the OsmMapWriter code. This creates
 * duplicate code, is inconsistent and difficult to maintain/use. The intent of this visitor is
 * to centralize the creation of these tags. This comes at the cost of cloning every element during
 * the export/write operation (to avoid modifying the input element), but I think it is likely
 * worth the cost.
 *
 * @todo Refactor all existing writers to use this visitor.
 * @todo in 3076 branch:
   - decide on universal status include rules:
      - as discussed, we are considering these options:
        - removal of TextStatus config altogether
        - no longer using the includeDebug flag
        - instead individual specific -D flags for each: status, id and circ.error
        - keep the special cases of (!isNode || (isNode && hasMappingTags))
          and (isRelation || (!isRelation && validStatus)) when deciding
          whether we add the tag even when the status flag is on
        - consider an additional verbose flag to turn everything on always,
          no matter node type, etc.
   - later (on hold):
      - set includeCircularError default to false
      - fix all tests
      - remove stringcompat and use string status enum always
      - fix all tests if possible
 */
class AddExportTagsVisitor : public ElementVisitor
{
public:

  AddExportTagsVisitor();
  ~AddExportTagsVisitor() = default;

  static QString className() { return "AddExportTagsVisitor"; }

  void visit(const ElementPtr& pElement) override;

  void overrideDebugSettings();

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Adds tags needed for exporting"; }

  void setIncludeHootInfo(bool includeInfo) { _includeDebug = includeInfo; }
  void setIncludeIds(bool includeIds) { _includeIds = includeIds; }
  void setIncludeCircularError(bool includeCircularError)
  { _includeCircularError = includeCircularError; }

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
