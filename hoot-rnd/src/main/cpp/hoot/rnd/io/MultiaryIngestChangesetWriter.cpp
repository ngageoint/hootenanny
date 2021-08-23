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
 * @copyright Copyright (C) 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#include "MultiaryIngestChangesetWriter.h"

// hoot
#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>

// Qt
#include <QStringBuilder>

using namespace geos::geom;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmChangeWriter, MultiaryIngestChangesetWriter)

MultiaryIngestChangesetWriter::MultiaryIngestChangesetWriter() :
_precision(16),
_elementPayloadFormat("json")
{
}

MultiaryIngestChangesetWriter::~MultiaryIngestChangesetWriter()
{
  close();
}

void MultiaryIngestChangesetWriter::setConfiguration(const Settings& conf)
{
  ConfigOptions options(conf);
  _precision = options.getWriterPrecision();
  _elementPayloadFormat = options.getSparkChangesetWriterElementPayloadFormat().toLower();
}

void MultiaryIngestChangesetWriter::open(const QString& fileName)
{
  close();

  _fp = std::make_shared<QFile>();
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
}

void MultiaryIngestChangesetWriter::writeChange(const Change& change)
{
  const long debugId = 6633775;

  if (change.getElement()->getElementType() != ElementType::Node)
  {
    throw NotImplementedException("Only nodes are supported.");
  }

  LOG_VART(change.getElement());

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

  if (Log::getInstance().getLevel() <= Log::Trace &&
      change.getElement()->getElementId().getId() == debugId)
  {
    LOG_VART(change.getElement());
  }

  NodePtr nodeCopy = (std::dynamic_pointer_cast<const Node>(change.getElement()))->cloneSp();

  nodeCopy->getTags().remove(MetadataTags::HootHash());
  _exportTagsVisitor.visit(nodeCopy);

  OsmMapPtr tmpMap = std::make_shared<OsmMap>();
  tmpMap->addElement(nodeCopy);

  QString changeLine;
  changeLine += changeType % "\t";

  //element payload

  if (_elementPayloadFormat == QLatin1String("json"))
  {
    // some of this may be redundant with what's in OsmJsonWriter and in SparkChangesetWriter

    //using elements in an array here, since that's what OsmJsonReader expects when using that
    //to parse (although we're not currently doing that with multiary ingest due to #1772)
    changeLine += "{\"elements\":[{\"type\":\"node\"";
    changeLine += ",\"id\":" % QString::number(nodeCopy->getId(), 'g', _precision);
    changeLine += ",\"lat\":" % QString::number(nodeCopy->getY(), 'g', _precision);
    changeLine += ",\"lon\":" % QString::number(nodeCopy->getX(), 'g', _precision);
    changeLine += ",\"tags\":{";
    bool first = true;
    const Tags& tags = nodeCopy->getTags();
    for (Tags::const_iterator it = tags.begin(); it != tags.end(); ++it)
    {
      const QString tagKey = it.key();
      const QString tagValue = it.value().trimmed();
      if (!tagValue.isEmpty())
      {
        if (!first)
        {
          changeLine += ",";
        }
        changeLine +=
          OsmJsonWriter::markupString(tagKey) % ":" % OsmJsonWriter::markupString(tagValue);
        first = false;
      }
    }
    changeLine += "}}]}\n";
  }
  else //xml
  {
    changeLine += OsmXmlWriter::toString(tmpMap, false);
  }

  LOG_VART(changeLine);

  if (_fp->write(changeLine.toUtf8()) == -1)
  {
    throw HootException("Error writing to file: " + _fp->errorString());
  }
}

void MultiaryIngestChangesetWriter::setElementPayloadFormat(const QString& format)
{
  if (format != "json" && format != "xml")
  {
    throw IllegalArgumentException("Invalid format: " + format);
  }
  _elementPayloadFormat = format;
}

}
