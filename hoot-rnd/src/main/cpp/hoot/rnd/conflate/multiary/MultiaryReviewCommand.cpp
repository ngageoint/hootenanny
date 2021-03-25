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

#include "MultiaryReviewCommand.h"

//  Std
#include <sstream>

//  Hoot
#include <hoot/core/io/OsmJsonReader.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/util/Log.h>
#include <hoot/rnd/visitors/MultiaryPoiHashVisitor.h>

// Boost
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#include <QCryptographicHash>

namespace pt = boost::property_tree;

namespace hoot
{

MultiaryReviewCommand::MultiaryReviewCommand():
  _op(NoOp),
  _finalElement()
{
  // Constructor
}

MultiaryReviewCommand::Operation MultiaryReviewCommand::getOp() const
{
  return _op;
}

void MultiaryReviewCommand::setOp(Operation op)
{
  _op = op;
}

QSet<QByteArray> MultiaryReviewCommand::getElementHashes() const
{
  return _elementHashes;
}

void MultiaryReviewCommand::setElementHashes(QSet<QByteArray> hashes)
{
  _elementHashes = hashes;
}

void MultiaryReviewCommand::clearElementHahses()
{
  _elementHashes.clear();
}

void MultiaryReviewCommand::addElementHash(QByteArray hash)
{
  _elementHashes.insert(hash);
}

void MultiaryReviewCommand::addElementHash(QString hashString)
{
  // Convert a string sha1 hash to QByteArray
  _elementHashes.insert(QByteArray::fromHex(hashString.toStdString().c_str()));
}

void MultiaryReviewCommand::addElementHash(ElementPtr e)
{
  _elementHashes.insert(_getElementHashID(e));
}

bool MultiaryReviewCommand::containsElement(ElementPtr e) const
{
  QByteArray hashID = _getElementHashID(e);
  return _elementHashes.contains(hashID);
}

bool MultiaryReviewCommand::containsElementHash(QByteArray hash) const
{
  return _elementHashes.contains(hash);
}

ElementPtr MultiaryReviewCommand::getFinalElement() const
{
  return _finalElement;
}

void MultiaryReviewCommand::setFinalElement(ElementPtr e)
{
  _finalElement = e;
}

// If this function seems slow, modify it to do fewer string conversions.
QString MultiaryReviewCommand::toJsonString() const
{
  // Put our element in a map to make it easy to write
  OsmMapPtr pMap(new OsmMap());
  if (_finalElement)
    pMap->addElement(_copyElement(_finalElement));

  // Get a json writer
  OsmJsonWriter jsonWriter;

  // Create the json
  std::ostringstream oss;
  oss << "{\n"
      << "   \"MultiaryReviewCommand\": {\n"
      << "   \"Operation\": \"" << operationToString(_op).toStdString() << "\",\n"
      << "   \"ElementHashes\": [" << _getHashIDList().toStdString() << "],\n"
      << "   \"FinalElement\": ";

  if (_finalElement)
    oss << jsonWriter.toString(pMap).toStdString() << "}";
  else
    oss << "{}}\n";

  oss << "}\n";

  LOG_TRACE(oss.str());
  return QString(oss.str().c_str());
}

QString MultiaryReviewCommand::toTSVString() const
{
  // Store json as our opaque payload
  QString jsonStr = toJsonString();
  jsonStr.replace("\n", " ");
  QString tsvStr = "";

  // Operation
  tsvStr += opToTsvAbbrev(_op) += "\t";

  // Comma-separated list of hash-ids
  tsvStr += _getHashIDList();
  tsvStr += "\t";

  // Payload - actual command
  tsvStr += jsonStr;

  // Endline!
  tsvStr += "\n";

  return tsvStr;
}

MultiaryReviewCommand MultiaryReviewCommand::fromJsonString(QString jsonStr)
{
  // Convert string to stringstream
  std::stringstream ss(jsonStr.toUtf8().constData(), std::ios::in);

  if (!ss.good())
  {
    throw HootException(QString("Error reading from JSON string:\n%1").arg(jsonStr));
  }

  // Property tree that holds JSON
  // Try to read the string
  boost::property_tree::ptree propTree;
  try
  {
    pt::read_json(ss, propTree);
  }
  catch (pt::json_parser::json_parser_error& e)
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

  MultiaryReviewCommand newCommand;

  // Get our operation
  propTree = propTree.get_child("MultiaryReviewCommand");
  QString OpStr = QString::fromStdString(propTree.get("Operation", std::string("")));
  newCommand.setOp(MultiaryReviewCommand::operationFromString(OpStr));


  // Get hashes
  pt::ptree hashes = propTree.get_child("ElementHashes");
  for (pt::ptree::const_iterator it = hashes.begin(); it != hashes.end(); ++it)
  {
    std::string hashStr = it->second.data();
    newCommand.addElementHash(QString::fromStdString(hashStr));
  }

  // Get the FinalElement
  if (MultiaryReviewCommand::Match == newCommand.getOp()
      || MultiaryReviewCommand::Modify == newCommand.getOp())
  {
    OsmMapPtr pMap = OsmJsonReader().loadFromPtree(propTree.get_child("FinalElement"));

    // Get the first node out of the map
    if (pMap->getNodeCount() > 0)
    {
      NodePtr pNode = pMap->getNodes().begin()->second;
      newCommand.setFinalElement(NodePtr(new Node(*pNode)));
    }
  }

  return newCommand;
}

bool MultiaryReviewCommand::operator==(const MultiaryReviewCommand& rhs) const
{
  // For now we compare json strings. If this is too slow, we could re-write
  // to do a deep object comparison
  QString thisJson = toJsonString();
  QString thatJson = rhs.toJsonString();
  return (0 == thisJson.compare(thatJson, Qt::CaseInsensitive));
}

bool MultiaryReviewCommand::applyToMap(OsmMapPtr pMap) const
{
  // TODO: Implement this!
  (void) pMap;
  return false;
}

QByteArray MultiaryReviewCommand::_getElementHashID(ElementPtr e) const
{
  QString hashKey = MetadataTags::HootHash();

  // Ensure the hash is present
  if (!e->getTags().contains(hashKey))
  {
    MultiaryPoiHashVisitor v;
    v.setIncludeCircularError(true);
    v.visit(e);
  }

  // Return the hash
  QString hexString = e->getTags()[hashKey];
  hexString.replace("sha1sum:", "");
  return QByteArray::fromHex(hexString.toLatin1());
}

QString MultiaryReviewCommand::_getHashIDList() const
{
  // Get a list of hashes
  QString hashStr = "";
  for (QSet<QByteArray>::ConstIterator it = _elementHashes.begin(); it != _elementHashes.end(); ++it)
  {
    hashStr +=  "\"" + QString(it->toHex()) + "\",";
  }
  hashStr.chop(1); // trailing comma
  return hashStr;
}

ElementPtr MultiaryReviewCommand::_copyElement (ElementPtr e) const
{
  switch(e->getElementType().getEnum())
  {
  case ElementType::Node:
    return ElementPtr(new Node(*(std::dynamic_pointer_cast<Node>(e))));
    break;
  case ElementType::Way:
    return ElementPtr(new Way(*(std::dynamic_pointer_cast<Way>(e))));
    break;
  case ElementType::Relation:
    return ElementPtr(new Relation(*(std::dynamic_pointer_cast<Relation>(e))));
    break;
  default:
    return ElementPtr();
  }
}

} // namespace hoot
