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
#include "SparkChangesetWriter.h"

// geos
#include <geos/geom/Envelope.h>

using namespace geos::geom;

// hoot
#include <hoot/core/conflate/MatchFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Exception.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MetadataTags.h>

// Qt
#include <QStringBuilder>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmChangeWriter, SparkChangesetWriter)

SparkChangesetWriter::SparkChangesetWriter() :
_precision(ConfigOptions().getWriterPrecision())
{
}

SparkChangesetWriter::~SparkChangesetWriter()
{
  close();
}

void SparkChangesetWriter::open(QString fileName)
{
  close();

  _fp.reset(new QFile());
  _fp->setFileName(fileName);
  if (_fp->exists() && !_fp->remove())
  {
    throw HootException(QObject::tr("Error removing existing %1 for writing.").arg(fileName));
  }
  if (!_fp->open(QIODevice::WriteOnly | QIODevice::Text))
  {
    throw HootException(QObject::tr("Error opening %1 for writing.").arg(fileName));
  }
  LOG_DEBUG("Opened: " << fileName << ".");

  // find a match creator that can provide the search bounds.
  foreach (boost::shared_ptr<MatchCreator> mc, MatchFactory::getInstance().getCreators())
  {
    SearchRadiusProviderPtr sbc = boost::dynamic_pointer_cast<SearchRadiusProvider>(mc);
    if (sbc.get())
    {
      if (_bounds.get())
      {
        LOG_WARN("Found more than one bounds calculator. Using the first one.");
      }
      else
      {
        _bounds.reset(new SearchBoundsCalculator(sbc));
      }
    }
  }

  if (!_bounds.get())
  {
    throw HootException(
      "You must specify one match creator that supports search radius calculation.");
  }
}

void SparkChangesetWriter::writeChange(const Change& change)
{
  if (change.getElement()->getElementType() != ElementType::Node)
  {
    throw NotImplementedException("Only nodes are supported.");
  }

  if (!change.getElement()->getTags().contains(MetadataTags::HootHash()))
  {
    throw IllegalArgumentException("No hash value set on element.");
  }

  if (change.getPreviousElement() &&
      !change.getPreviousElement()->getTags().contains(MetadataTags::HootHash()))
  {
    throw IllegalArgumentException("No hash value set on previous element.");
  }

  LOG_VART(change);

  QString changeType;
  switch (change.getType())
  {
    case Change::Create:
      changeType = "A";
      break;
    case Change::Modify:
      changeType = "M";
      break;
    case Change::Delete:
      changeType = "D";
      break;
    default:
      throw IllegalArgumentException("Unexpected change type.");
  }

  ConstNodePtr node = boost::dynamic_pointer_cast<const Node>(change.getElement());
  NodePtr nodeCopy(dynamic_cast<Node*>(node->clone()));
  const QString nodeHash = nodeCopy->getTags()[MetadataTags::HootHash()];
  nodeCopy->getTags().remove(MetadataTags::HootHash());
  _exportTagsVisitor.visit(nodeCopy);
  OsmMapPtr tmpMap(new OsmMap());
  tmpMap->addElement(nodeCopy);

  Envelope env = _bounds->calculateSearchBounds(tmpMap, nodeCopy);

  QString changeLine;
  changeLine +=
    changeType % "\t" %
    QString::number(env.getMinX(), 'g', _precision) % "\t" %
    QString::number(env.getMinY(), 'g', _precision) % "\t" %
    QString::number(env.getMaxX(), 'g', _precision) % "\t" %
    QString::number(env.getMaxY(), 'g', _precision) % "\t";
  if (change.getType() == Change::Modify)
  {
    if (!change.getPreviousElement().get())
    {
      throw HootException("No previous element specified for modify change.");
    }
    else if (change.getPreviousElement()->getElementType() != ElementType::Node)
    {
      throw NotImplementedException("Only nodes are supported.");
    }
    ConstNodePtr previousNode =
      boost::dynamic_pointer_cast<const Node>(change.getPreviousElement());
    NodePtr previousNodeCopy(dynamic_cast<Node*>(previousNode->clone()));
    _exportTagsVisitor.visit(previousNodeCopy);
    // element hash before change
    changeLine += previousNodeCopy->getTags()[MetadataTags::HootHash()] % "\t";
  }
  changeLine += nodeHash % "\t";  // element hash after change

  //element payload

  changeLine += "{\"element\":{\"type\":\"node\"";
  changeLine += ",\"id\":" % QString::number(nodeCopy->getId(), 'g', _precision);
  changeLine += ",\"lat\":" % QString::number(nodeCopy->getY(), 'g', _precision);
  changeLine += ",\"lon\":" % QString::number(nodeCopy->getX(), 'g', _precision);
  changeLine += ",\"tags\":{";
  bool first = true;
  const Tags& tags = nodeCopy->getTags();
  for (Tags::const_iterator it = tags.begin(); it != tags.end(); ++it)
  {
    const QString tagValue = it.value();
    if (!tagValue.trimmed().isEmpty())
    {
      if (!first)
      {
        changeLine += ",";
      }
      changeLine +=
        OsmJsonWriter::markupString(it.key()) % ":" % OsmJsonWriter::markupString(tagValue);
      first = false;
    }
  }
  changeLine += "}}}\n";
  LOG_VART(changeLine);

  if (_fp->write(changeLine.toUtf8()) == -1)
  {
    throw HootException("Error writing to file: " + _fp->errorString());
  }
}

}
