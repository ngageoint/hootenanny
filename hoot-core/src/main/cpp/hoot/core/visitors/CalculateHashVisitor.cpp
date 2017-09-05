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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "CalculateHashVisitor.h"

// hoot
#include <hoot/core/conflate/ReviewMarker.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QCryptographicHash>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, CalculateHashVisitor)

CalculateHashVisitor::~CalculateHashVisitor()
{
}

QString CalculateHashVisitor::toJsonString(const ConstElementPtr& e)
{
  if (e->getElementType() != ElementType::Node)
  {
    throw NotImplementedException("Only nodes are supported at this time.");
  }

  ConstNodePtr n = boost::dynamic_pointer_cast<const Node>(e);

  QString result = "{\"type\":\"Feature\",\"properties\":{\"type\":\"node\",\"tags\":{";

  // put the tags into an ordered map that only contains the non-metadata (info) tags. As
  // implemented this is likely quite slow.
  QMap<QString, QString> infoTags;
  foreach (QString key, e->getTags().keys())
  {
    QString v = e->getTags()[key];
    if (OsmSchema::getInstance().isMetaData(key, v) == false)
    {
      infoTags[key] = v;
    }
  }

  if (n->getRawCircularError() >= 0)
  {
    infoTags["error:circular"] =
      QString::number(
        n->getRawCircularError(), 'g', ConfigOptions().getNodeComparisonCircularErrorSensitivity());
  }

  bool first = true;
  foreach (QString key, infoTags.keys())
  {
    if (!first)
    {
      result += ",";
    }
    result += QString("\"%1\":\"%2\"").arg(key, infoTags[key]);
    first = false;
  }

  result += "}},\"geometry\":{\"type\":\"Point\",\"coordinates\":[";
  result +=
    QString::number(n->getX(), 'f', ConfigOptions().getNodeComparisonCoordinateSensitivity());
  result += ",";
  result +=
    QString::number(n->getY(), 'f', ConfigOptions().getNodeComparisonCoordinateSensitivity());
  result += "]}}";

  return result;
}

QByteArray CalculateHashVisitor::toHash(const ConstElementPtr& e)
{
  QCryptographicHash hash(QCryptographicHash::Sha1);

  hash.addData(toJsonString(e).toAscii().constData());

  return hash.result();
}

QString CalculateHashVisitor::toHashString(const ConstElementPtr& e)
{
  return QString::fromUtf8(toHash(e).toHex());
}

void CalculateHashVisitor::visit(const ElementPtr& e)
{
  // don't calculate hashes on review relations.
  if (ReviewMarker::isReview(e) == false)
  {
    QByteArray hash = toHash(e);

    e->getTags()[MetadataTags::HootHash()] = "sha1sum:" + QString::fromUtf8(hash.toHex());
  }
}

}
