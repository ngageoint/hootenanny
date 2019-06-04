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
#ifndef APITAGTRUNCATEVISITOR_H
#define APITAGTRUNCATEVISITOR_H

//  Hoot
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/info/OperationStatusInfo.h>

namespace hoot
{

class ApiTagTruncateVisitor : public ElementVisitor, public OperationStatusInfo
{
public:

  static std::string className() { return "hoot::ApiTagTruncateVisitor"; }

  ApiTagTruncateVisitor() { }

  virtual void visit(const ElementPtr& e) override;

  virtual QString getDescription() const override { return "Truncates tag key/value pairs to the API limit of 255 characters"; }

  virtual QString getInitStatusMessage() const override
  { return "Truncating tag key/value pairs for OSM API..."; }

  virtual QString getCompletedStatusMessage() const override
  { return "Truncated tag key/value pairs for " + QString::number(_numAffected) + " elements"; }

  /**
   * @brief truncateTags Does the actual truncating of tags
   * @param tags Input and output tags object
   * @return true if any of the tags were modified
   */
  static bool truncateTags(Tags& tags);

  /**
   * @brief truncateTag
   * @param key
   * @param value
   * @return
   */
  static QString truncateTag(const QString& key, const QString& value);

};

}
#endif  //  APITAGTRUNCATEVISITOR_H
