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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "NetUtils.h"

// Hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/HootException.h>

namespace hoot
{

void NetUtils::checkWebReplyForError(QNetworkReply* reply)
{
  QVariant status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
  LOG_VARD(status.isValid());
  LOG_VARD(status.toInt());
  if (reply->error() != QNetworkReply::NoError)
  {
    throw HootException(QString("Languages reply error:\n%1").arg(reply->error()));
  }
}

boost::shared_ptr<boost::property_tree::ptree> NetUtils::replyToPropTree(QNetworkReply* reply)
{
  QString replyData(reply->readAll());
  LOG_VART(replyData);
  std::stringstream replyStrStrm(replyData.toUtf8().constData(), std::ios::in);
  if (!replyStrStrm.good())
  {
    throw HootException(QString("Error reading from reply string:\n%1").arg(replyData));
  }
  boost::shared_ptr<boost::property_tree::ptree> replyObj(new boost::property_tree::ptree());
  try
  {
    boost::property_tree::read_json(replyStrStrm, *replyObj);
  }
  catch (boost::property_tree::json_parser::json_parser_error& e)
  {
    QString reason = QString::fromStdString(e.message());
    QString line = QString::number(e.line());
    throw HootException(QString("Error parsing JSON: %1 (line %2)").arg(reason).arg(line));
  }
  catch (const std::exception& e)
  {
    QString reason = e.what();
    throw HootException("Error parsing JSON " + reason);
  }
  return replyObj;
}

}
