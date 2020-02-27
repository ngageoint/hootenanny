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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef APITAGTRUNCATEVISITOR_H
#define APITAGTRUNCATEVISITOR_H

//  Hoot
#include <hoot/core/elements/ElementVisitor.h>

namespace hoot
{

/**
 * @brief The ApiTagTruncateVisitor class truncates tags that are going to be passed into
 *  an OSM API database.  The API imposes a 255 character limit to tag values even though
 *  the database doesn't.  All tags have this limit but three tags (so far) require special
 *  processing.  First, lists aren't truncated mid-listitem, the last item that starts but
 *  doesn't end before the 255 limit is removed along with all items after that.  Second,
 *  `source:datetime` and `source:ingest:datetime` are date lists that are truncated down
 *  to include only the last date in the list.  Thirdly the `uuid` field is handled the
 *  same way, only the last UUID in the list is preserved.
 */
class ApiTagTruncateVisitor : public ElementVisitor
{
public:

  static std::string className() { return "hoot::ApiTagTruncateVisitor"; }

  ApiTagTruncateVisitor() { }

  virtual void visit(const ElementPtr& e) override;

  virtual QString getDescription() const override
  { return "Truncates tag key/value pairs to the API limit of 255 characters"; }

  virtual QString getInitStatusMessage() const override
  { return "Truncating tag key/value pairs for OSM API..."; }

  virtual QString getCompletedStatusMessage() const override
  { return "Truncated tag key/value pairs for " + QString::number(_numAffected) + " elements"; }

  /**
   * @brief truncateTags Iterates all tags calling truncateTag one by one
   * @param tags Input and output tags object
   * @return true if any of the tags were modified
   */
  static bool truncateTags(Tags& tags);

  /**
   * @brief truncateTag Does the actual truncating of a tag
   * @param key Tag key (some tags require special processing)
   * @param value Tag value to be truncated if > 255 in length
   * @return Empty string if nothing is changed, truncated string otherwise
   */
  static QString truncateTag(const QString& key, const QString& value);

  virtual std::string getClassName() const { return className(); }

};

}
#endif  //  APITAGTRUNCATEVISITOR_H
