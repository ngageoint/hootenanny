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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "TagInfo.h"

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/elements/ElementIterator.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/io/OgrReader.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QFileInfo>

namespace hoot
{

TagInfo::TagInfo(
  const int tagValuesPerKeyLimit, const QStringList& keys, const bool keysOnly,
  const bool caseSensitive, const bool exactKeyMatch, const bool delimitedTextOutput) :
_tagValuesPerKeyLimit(tagValuesPerKeyLimit),
_keys(keys),
_keysOnly(keysOnly),
_caseSensitive(caseSensitive),
_exactKeyMatch(exactKeyMatch),
_delimitedTextOutput(delimitedTextOutput),
_taskStatusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval())
{
  if (delimitedTextOutput && !keysOnly)
  {
    throw IllegalArgumentException("Delimited text output is only valid when listing keys only.");
  }
}

QString TagInfo::getInfo(const QStringList& inputs) const
{
  QString info;

  if (_delimitedTextOutput)
  {
    QSet<QString> uniqueKeys;
    for (int i = 0; i < inputs.size(); i++)
    {
      info += _getInfo(inputs.at(i));

      const QSet<QString> keys = _getInfo(inputs.at(i)).split(";").toSet();
      for (QSet<QString>::const_iterator it = keys.begin(); it != keys.end(); ++it)
      {
        const QString key = *it;
        if (!key.isEmpty())
        {
          uniqueKeys.insert(key);
        }
      }
    }
    QStringList keyList = uniqueKeys.toList();
    keyList.sort();
    return keyList.join(";");
  }
  else
  {
    info = "{\n";

    for (int i = 0; i < inputs.size(); i++)
    {
      info += QString("  \"%1\":{\n").arg(QFileInfo(inputs.at(i)).fileName());

      info += _getInfo(inputs.at(i));
      info += "\n  }";

      // Don't add a comma to the last dataset.
      if (i != (inputs.size() - 1))
      {
        info += ",\n";
      }
    }
    info += "\n}";
  }

  return info;
}

QString TagInfo::_getInfo(const QString& input) const
{
  LOG_VART(_tagValuesPerKeyLimit);
  LOG_VART(_keys);
  LOG_VART(_keysOnly);
  LOG_VART(_caseSensitive);
  LOG_VART(_exactKeyMatch);

  QString inputInfo = input;
  LOG_VARD(inputInfo);

  std::shared_ptr<OsmMapReader> reader =
    OsmMapReaderFactory::createReader(
      inputInfo, ConfigOptions().getReaderUseDataSourceIds(),
      Status::fromString(ConfigOptions().getReaderSetDefaultStatus()));
  std::shared_ptr<OgrReader> ogrReader = std::dynamic_pointer_cast<OgrReader>(reader);
  if (ogrReader.get())
  {
    // Using a different code path for the OGR inputs to handle the layer syntax. We need to add
    // custom behavior to the element parsing, so loading the map through IoUtils::loadMap won't work
    // here.
    return _getInfoFromOgrInput(ogrReader, inputInfo);
  }
  else
  {
    return _getInfoFromStreamableInput(reader, inputInfo);
  }
}

QString TagInfo::_getInfoFromOgrInput(
  const std::shared_ptr<OgrReader>& reader, QString& input) const
{
  // We have to have a translation for the reading, so just use the simplest one.
  reader->setSchemaTranslationScript(ConfPath::getHootHome() + "/translations/quick.js");

  QStringList layers;
  if (input.contains(";"))
  {
    QStringList list = input.split(";");
    input = list.at(0);
    layers.append(list.at(1));
  }
  else
  {
    layers = reader->getFilteredLayerNames(input);
  }

  if (layers.empty())
  {
    LOG_WARN("Could not find any valid layers to read from in " + input + ".");
  }

  QString finalText;
  for (int i = 0; i < layers.size(); i++)
  {
    LOG_DEBUG("Reading: " << input + " " << layers[i] << "...");

    TagInfoHash result;
    int numElementsProcessed = 0;
    std::shared_ptr<ElementIterator> iterator(reader->createIterator(input, layers[i]));
    while (iterator->hasNext())
    {
      std::shared_ptr<Element> e = iterator->next();
      _parseElement(e, result);

      numElementsProcessed++;
      if (numElementsProcessed % (_taskStatusUpdateInterval * 10) == 0)
      {
        PROGRESS_INFO(
          "Processed " << StringUtils::formatLargeNumber(numElementsProcessed) << " elements.");
      }
    }

    if (_delimitedTextOutput)
    {
      const QString tmpText = _printDelimitedText(result);
      // Skip empty layers.
      if (tmpText == "")
      {
        continue;
      }
      finalText += tmpText;
      if (i != (layers.size() - 1))
      {
        finalText += ";";
      }
    }
    else
    {
      const QString tmpText = _printJSON(layers[i], result);
      // Skip empty layers.
      if (tmpText == "")
      {
        continue;
      }
      finalText += tmpText;
      if (i != (layers.size() - 1))
      {
        finalText += ",\n";
      }
    }
  }
  return finalText;
}

QString TagInfo::_getInfoFromStreamableInput(
  const std::shared_ptr<OsmMapReader>& reader, const QString& input) const
{
  // At this time, the only unstreamable readers are the JSON readers. If this capability is
  // needed for JSON data, then either those readers can implement PartialOsmMapReader or the
  // needed readed code can be manually added to this class.
  // TODO: change this
  if (!IoUtils::isStreamableInput(input))
  {
    throw IllegalArgumentException("Inputs to TagInfo must be streamable.");
  }

  LOG_DEBUG("Reading: " << input << "...");

  reader->open(input);
  std::shared_ptr<ElementInputStream> streamReader =
    std::dynamic_pointer_cast<ElementInputStream>(reader);

  TagInfoHash result;
  int numElementsProcessed = 0;
  while (streamReader->hasMoreElements())
  {
    ElementPtr e = streamReader->readNextElement();
    if (e.get())
    {
      LOG_VART(e);
      _parseElement(e, result);
    }

    numElementsProcessed++;
    if (numElementsProcessed % (_taskStatusUpdateInterval * 10) == 0)
    {
      PROGRESS_INFO(
        "Processed " << StringUtils::formatLargeNumber(numElementsProcessed) << " elements.");
    }
  }
  std::shared_ptr<PartialOsmMapReader> partialReader =
    std::dynamic_pointer_cast<PartialOsmMapReader>(reader);
  if (partialReader.get())
  {
    partialReader->finalizePartial();
  }

  if (_delimitedTextOutput)
  {
    return _printDelimitedText(result);
  }
  else
  {
    return _printJSON("osm", result);
  }
}

bool TagInfo::_tagKeysMatch(const QString& tagKey) const
{
  LOG_VART(tagKey);
  const Qt::CaseSensitivity caseSensitivity =
    _caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;
  if (_exactKeyMatch)
  {
    return _keys.contains(tagKey, caseSensitivity);
  }
  else
  {
    for (int i = 0; i < _keys.size(); i++)
    {
      const QString specifiedTagKey = _keys.at(i);
      LOG_VART(specifiedTagKey);
      if (tagKey.contains(specifiedTagKey, caseSensitivity))
      {
        return true;
      }
    }
  }
  return false;
}

void TagInfo::_parseElement(const ElementPtr& e, TagInfoHash& result) const
{
  for (Tags::const_iterator it = e->getTags().begin(); it != e->getTags().end(); ++it)
  {
    LOG_VART(it.key());
    LOG_VART(it.value());

    if (it.value() == "") // Drop empty values
    {
      continue;
    }

    // Drop Hoot metadata tags
    if (it.key() == MetadataTags::SourceIngestDateTime())
    {
      continue;
    }

    LOG_VART(result.value(it.key()));
    LOG_VART(result.value(it.key()).size());
    if (result.value(it.key()).size() < _tagValuesPerKeyLimit)
    {
      result[it.key()][it.value()]++;
    }
  }
}

QString TagInfo::_printDelimitedText(TagInfoHash& data) const
{
  assert(_keysOnly);

  QStringList attrKey = data.keys();
  // Skip empty layers
  if (attrKey.count() == 0)
  {
    return "";
  }

  attrKey.sort(); // Sort the attribute list to make it look better

  QString result;
  for (int i = 0; i < attrKey.count(); i++)
  {
    const QString key = attrKey[i];
    if (!key.isEmpty())
    {
      result += key + ";";
    }
  }
  return result;
}

QString TagInfo::_printJSON(const QString& lName, TagInfoHash& data) const
{
  QStringList attrKey = data.keys();
  // Skip empty layers
  if (attrKey.empty())
  {
    return "";
  }

  attrKey.sort(); // Sort the attribute list to make it look better

  QString result;

  // Start the JSON
  if (!_keysOnly)
  {
    result += QString("    \"%1\":{\n").arg(lName);
  }
  else
  {
    result += QString("    \"%1\":[\n").arg(lName);
  }

  int keysParsed = 0;
  for (int i = 0; i < attrKey.count(); i++)
  {
    QStringList valKey = data[attrKey[i]].keys();

    int maxValues = valKey.count();
    valKey.sort();

    const QString key = attrKey[i];
    if (_keys.isEmpty() || _tagKeysMatch(key))
    {
      if (!_keysOnly)
      {
        result += QString("      \"%1\":[\n").arg(key);

        for (int j = 0; j < maxValues; j++)
        {
          QString tmpVal(valKey[j]);

          // Escape carriage returns
          tmpVal.replace("\n","\\n");
          // Escape linefeeds
          tmpVal.replace("\r","\\r");
          // Escape form feeds
          tmpVal.replace("\f","\\f");
          // Escape tabs
          tmpVal.replace("\t","\\t");
          // Escape vertical tabs
          tmpVal.replace("\v","\\v");
          // And double quotes
          tmpVal.replace("\"","\\\"");

          result += QString("        \"%1\"").arg(tmpVal);

          if (j != (maxValues - 1))
          {
            result += ",\n";
          }
          else
          {
            // No comma after bracket
            result += "\n";
          }
        }

        if (i != (attrKey.count() - 1))
        {
          result += "        ],\n";
        }
        else
        {
          // No comma after bracket
          result += "        ]\n";
        }
      }
      else
      {
        if (i != (attrKey.count() - 1))
        {
          result += QString("      \"%1\",\n").arg(key);
        }
        else
        {
          result += QString("      \"%1\"\n      ]\n").arg(key);
        }
      }
      keysParsed++;
    }
  }

  if (keysParsed == 0)
  {
    return "No keys found from input key list.";
  }

  if (!_keysOnly)
  {
    // We have no idea if this is the last layer, so no comma on the end
    result += "      }";
  }

  // A bit hackish to handle the situation wher the last key enountered isn't in the specified
  // keys list and avoid an unneeded trailing comma.
  result.replace("        ],\n      }", "        ]\n      }");

  return result;
}

}
