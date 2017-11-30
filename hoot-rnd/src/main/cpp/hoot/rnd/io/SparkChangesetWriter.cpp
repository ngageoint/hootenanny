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
#include <hoot/rnd/conflate/multiary/MultiaryUtilities.h>

// Qt
#include <QStringBuilder>
#include <QFileInfo>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmChangeWriter, SparkChangesetWriter)

SparkChangesetWriter::SparkChangesetWriter() :
_precision(16)
{
}

SparkChangesetWriter::~SparkChangesetWriter()
{
  close();
}

void SparkChangesetWriter::setConfiguration(const Settings& conf)
{
  ConfigOptions options(conf);
  _precision = options.getWriterPrecision();
}

void SparkChangesetWriter::open(QString fileName)
{
  close();

  QFileInfo fileInfo(fileName);

  _addFile.reset(new QFile());
  const QString addFileName =
    fileInfo.absolutePath() + "/" + fileInfo.baseName() + "-add." + fileInfo.completeSuffix();
  _addFile->setFileName(addFileName);
  if (_addFile->exists() && !_addFile->remove())
  {
    throw HootException(QObject::tr("Error removing existing %1 for writing.").arg(addFileName));
  }
  if (!_addFile->open(QIODevice::WriteOnly | QIODevice::Text))
  {
    throw HootException(QObject::tr("Error opening %1 for writing.").arg(addFileName));
  }
  LOG_DEBUG("Opened: " << addFileName << ".");

  _deleteFile.reset(new QFile());
  const QString deleteFileName =
    fileInfo.absolutePath() + "/" + fileInfo.baseName() + "-delete." + fileInfo.completeSuffix();
  _deleteFile->setFileName(deleteFileName);
  if (_deleteFile->exists() && !_deleteFile->remove())
  {
    throw HootException(QObject::tr("Error removing existing %1 for writing.").arg(deleteFileName));
  }
  if (!_deleteFile->open(QIODevice::WriteOnly | QIODevice::Text))
  {
    throw HootException(QObject::tr("Error opening %1 for writing.").arg(deleteFileName));
  }
  LOG_DEBUG("Opened: " << deleteFileName << ".");

  // find a match creator that can provide the search bounds.
  foreach (boost::shared_ptr<MatchCreator> mc, MatchFactory::getInstance().getCreators())
  {
    //TODO: Why is ScriptMatchVisitor::calculateSearchRadius getting called a ton of times by this?
    SearchRadiusProviderPtr sbc = boost::dynamic_pointer_cast<SearchRadiusProvider>(mc);
    if (sbc.get())
    {
      if (_boundsCalculator.get())
      {
        LOG_WARN("Found more than one bounds calculator. Using the first one.");
      }
      else
      {
        _boundsCalculator.reset(new SearchBoundsCalculator(sbc));
      }
    }
  }

  if (!_boundsCalculator.get())
  {
    throw HootException(
      "You must specify one match creator that supports search radius calculation.");
  }
}

void SparkChangesetWriter::close()
{
  if (_addFile.get())
  {
    _addFile->close();
    _addFile.reset();
  }

  if (_deleteFile.get())
  {
    _deleteFile->close();
    _deleteFile.reset();
  }
}

void SparkChangesetWriter::writeChange(const Change& change)
{
  const long debugId = 6633775;

  LOG_VART(change);

  if (change.getType() == Change::Unknown)
  {
    throw IllegalArgumentException(QString("Invalid change type: ") + change.toString());
  }

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
  }

  if (Log::getInstance().getLevel() <= Log::Trace &&
      change.getElement()->getElementId().getId() == debugId)
  {
    LOG_VART(change.getElement());
  }

  NodePtr nodeCopy = (boost::dynamic_pointer_cast<const Node>(change.getElement()))->cloneSp();
  const QString nodeHash = nodeCopy->getTags()[MetadataTags::HootHash()];

  QString createChangeLine;
  QString deleteChangeLine;
  if (change.getType() == Change::Create || change.getType() == Change::Modify)
  {
    nodeCopy->getTags().remove(MetadataTags::HootHash());
    _exportTagsVisitor.visit(nodeCopy);

    OsmMapPtr tmpMap(new OsmMap());
    tmpMap->addElement(nodeCopy);

    const Envelope env = _boundsCalculator->calculateSearchBounds(tmpMap, nodeCopy);

    createChangeLine +=
      QString::number(env.getMinX(), 'g', _precision) % "\t" %
      QString::number(env.getMinY(), 'g', _precision) % "\t" %
      QString::number(env.getMaxX(), 'g', _precision) % "\t" %
      QString::number(env.getMaxY(), 'g', _precision) % "\t";

    createChangeLine += nodeHash % "\t";  // element hash after change

    //element payload

    //TODO: some of this may be redundant with what's in OsmJsonWriter and in
    //MultiaryIngestChangesetWriter

//    //using elements in an array here, since that's what OsmJsonReader expects when using that
//    //to parse (although we're not currently doing that with multiary ingest due to #1772)
//    createChangeLine += "{\"elements\":[{\"type\":\"node\"";
//    createChangeLine += ",\"id\":" % QString::number(nodeCopy->getId(), 'g', _precision);
//    createChangeLine += ",\"lat\":" % QString::number(nodeCopy->getY(), 'g', _precision);
//    createChangeLine += ",\"lon\":" % QString::number(nodeCopy->getX(), 'g', _precision);
//    createChangeLine += ",\"tags\":{";
//    bool first = true;
//    const Tags& tags = nodeCopy->getTags();
//    for (Tags::const_iterator it = tags.begin(); it != tags.end(); ++it)
//    {
//      const QString tagKey = it.key();
//      const QString tagValue = it.value().trimmed();
//      if (!tagValue.isEmpty())
//      {
//        if (!first)
//        {
//          createChangeLine += ",";
//        }
//        createChangeLine +=
//          OsmJsonWriter::markupString(tagKey) % ":" % OsmJsonWriter::markupString(tagValue);
//        first = false;
//      }
//    }
//    createChangeLine += "}}]}\n";
    createChangeLine +=
      /*"sha1sum:" +*/
      /*QString::fromUtf8(MultiaryUtilities::convertElementToPbf(nodeCopy).toHex())*/
      QString(MultiaryUtilities::convertElementToPbf(nodeCopy).toBase64().data()) +
      "\n";

    if (change.getType() == Change::Modify)
    {
      NodePtr previousNodeCopy =
        (boost::dynamic_pointer_cast<const Node>(change.getPreviousElement()))->cloneSp();

      _exportTagsVisitor.visit(previousNodeCopy); //TODO: is this needed?
      // element hash before change
      deleteChangeLine += QString(previousNodeCopy->getTags()[MetadataTags::HootHash()]) + "\n";
    }
  }
  //delete
  else
  {
    deleteChangeLine += nodeHash + "\n";
  }

  LOG_VART(createChangeLine);
  LOG_VART(deleteChangeLine);

  if (!createChangeLine.isEmpty() && _addFile->write(createChangeLine.toUtf8()) == -1)
  {
    throw HootException("Error writing to file: " + _addFile->errorString());
  }
  if (!deleteChangeLine.isEmpty() && _deleteFile->write(deleteChangeLine.toUtf8()) == -1)
  {
    throw HootException("Error writing to file: " + _deleteFile->errorString());
  }
}

void SparkChangesetWriter::setElementPayloadFormat(const QString format)
{
  if (format != "json")
  {
    throw IllegalArgumentException("Invalid format: " + format);
  }
}

}
