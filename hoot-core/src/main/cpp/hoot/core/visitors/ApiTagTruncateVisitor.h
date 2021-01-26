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
 * @copyright Copyright (C) 2019, 2020, 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef APITAGTRUNCATEVISITOR_H
#define APITAGTRUNCATEVISITOR_H

//  Hoot
#include <hoot/core/visitors/ElementVisitor.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/StringUtils.h>

namespace hoot
{

/**
 * @brief The ApiTagTruncateVisitor class truncates tags that are going to be passed into
 *  an OSM API database.  The OSM API imposes a 255 character limit to tag values even though
 *  the database doesn't.  Allow this class to specify its own upper limit for custom OSM API
 *  instances.  All tags have this limit but three tags (so far) require special
 *  processing.  First, lists aren't truncated mid-listitem, the last item that starts but
 *  doesn't end before the upper limit is removed along with all items after that.  Second,
 *  `source:datetime` and `source:ingest:datetime` are date lists that are truncated down
 *  to include only the last date in the list.  Thirdly the `uuid` field is handled the
 *  same way, only the last UUID in the list is preserved.
 */
class ApiTagTruncateVisitor : public ElementVisitor, public Configurable
{
public:

  static QString className() { return "hoot::ApiTagTruncateVisitor"; }

  ApiTagTruncateVisitor();
  virtual ~ApiTagTruncateVisitor() = default;

  void visit(const ElementPtr& e) override;

  QString getDescription() const override
  { return QString("Truncates tag key/value pairs to the API limit of %1 characters").arg(_maxLength); }

  QString getInitStatusMessage() const override
  { return "Truncating tag key/value pairs for OSM API..."; }

  QString getCompletedStatusMessage() const override
  {
    return
      "Truncated tag key/value pairs for " + StringUtils::formatLargeNumber(_numAffected) +
      " elements";
  }

  virtual QString getName() const override { return className(); }

  virtual QString getClassName() const override { return className(); }

  void setConfiguration(const Settings& conf) override;

  /**
   * @brief truncateTags Iterates all tags calling truncateTag one by one
   * @param tags Input and output tags object
   * @return true if any of the tags were modified
   */
  static bool truncateTags(Tags& tags);

  /**
   * @brief truncateTag Does the actual truncating of a tag
   * @param key Tag key (some tags require special processing)
   * @param value Tag value to be truncated if > _maxLength in length
   * @return Empty string if nothing is changed, truncated string otherwise
   */
  static QString truncateTag(const QString& key, const QString& value);

private:

  QString _truncateTag(const QString& key, const QString& value);
  bool _truncateTags(Tags& tags);

  int _maxLength;
};

}
#endif  //  APITAGTRUNCATEVISITOR_H
