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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ApiTagTruncateVisitor.h"

//  Hoot
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, ApiTagTruncateVisitor)

void ApiTagTruncateVisitor::visit(const ElementPtr& e)
{
  Tags& tags = e->getTags();
  bool elementAffected = truncateTags(tags);
  //  Update the statistics
  _numProcessed++;
  if (elementAffected)
    _numAffected++;
}

bool ApiTagTruncateVisitor::truncateTags(Tags& tags)
{
  bool tagsAffected = false;
  //  Iterate all tags looking for ones that are too long or the special cases
  for (Tags::iterator it = tags.begin(); it != tags.end(); ++it)
  {
    const QString& key = it.key();
    const QString& value = it.value();

    QString newValue = truncateTag(key, value);

    if (newValue != "")
    {
      it.value() = newValue;
      tagsAffected = true;
    }
  }
  return tagsAffected;
}

QString ApiTagTruncateVisitor::truncateTag(const QString &key, const QString &value)
{
  QString result;
  //  First check the special cases of lists where only one value is kept
  if (key == MetadataTags::SourceDateTime() ||
      key == MetadataTags::SourceIngestDateTime() ||
      key == "uuid")
  {
    //  These are special cases where only the last value in the list is kept
    QStringList values = value.split(";");
    if (values.count() > 1)
      result = values.last();
  }
  else if (value.length() > 255)
  {
    //  Truncate values greater than 255
    if (value.contains(";"))
    {
      //  These values are lists that will be truncated at a split point before or at the 255 mark
      int index = value.lastIndexOf(";", 255);
      if (index == -1)
        index = 255;
      //  Truncate at the index
      result = value.left(index);
    }
    else  //  Just truncate after character 255
      result = value.left(255);
  }
  return result;
}

}
