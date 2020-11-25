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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "TagInfo.h"

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/elements/ElementIterator.h>
#include <hoot/core/io/OgrReader.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QFileInfo>

namespace hoot
{

TagInfo::TagInfo(const int tagValuesPerKeyLimit, const QStringList& keys, const bool keysOnly,
                 const bool caseSensitive, const bool exactKeyMatch) :
_tagValuesPerKeyLimit(tagValuesPerKeyLimit),
_keys(keys),
_keysOnly(keysOnly),
_caseSensitive(caseSensitive),
_exactKeyMatch(exactKeyMatch),
_taskStatusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval())
{
}

QString TagInfo::getInfo(const QStringList& inputs)
{
  QString info = "{\n";

  for (int i = 0; i < inputs.size(); i++)
  {
    info += QString("  \"%1\":{\n").arg(QFileInfo(inputs.at(i)).fileName());

    info += _getInfo(inputs.at(i));
    info += "\n  }";

    // Dont add a comma to the last dataset
    if (i != (inputs.size() - 1))
    {
      info += ",\n";
    }
  }
  info += "\n}";

  return info;
}

QString TagInfo::_getInfo(const QString& input)
{
  QString inputInfo = input;
  LOG_VARD(inputInfo);
  LOG_VART(_tagValuesPerKeyLimit);
  LOG_VART(_keys);
  LOG_VART(_keysOnly);
  LOG_VART(_caseSensitive);
  LOG_VART(_exactKeyMatch);
  QString finalText;

  std::shared_ptr<OsmMapReader> reader =
    OsmMapReaderFactory::createReader(
      inputInfo, ConfigOptions().getReaderUseDataSourceIds(),
      Status::fromString(ConfigOptions().getReaderSetDefaultStatus()));

  //Using a different code path for the OGR inputs to handle the layer syntax.  There may be
  //a way to combine the two logic paths...not sure, though.
  std::shared_ptr<OgrReader> ogrReader = std::dynamic_pointer_cast<OgrReader>(reader);
  if (ogrReader.get())
  {
    ogrReader->setSchemaTranslationScript(QString(getenv("HOOT_HOME")) + "/translations/quick.js");

    QStringList layers;
    if (inputInfo.contains(";"))
    {
      QStringList list = inputInfo.split(";");
      inputInfo = list.at(0);
      layers.append(list.at(1));
    }
    else
    {
      layers = ogrReader->getFilteredLayerNames(inputInfo);
    }

    if (layers.size() == 0)
    {
      LOG_WARN("Could not find any valid layers to read from in " + inputInfo + ".");
    }

    for (int i = 0; i < layers.size(); i++)
    {
      LOG_DEBUG("Reading: " << inputInfo + " " << layers[i] << "...");

      TagInfoHash result;
      int numElementsProcessed = 0;
      std::shared_ptr<ElementIterator> iterator(ogrReader->createIterator(inputInfo, layers[i]));
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

      const QString tmpText = _printJSON(layers[i], result);
      // Skip empty layers
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
  else
  {
    // At this time, the only unstreamable readers are the JSON readers.  If this capability is
    // needed for JSON data, then either those readers can implement PartialOsmMapReader or the
    // needed readed code can be manually added to this class.

    if (!OsmMapReaderFactory::hasElementInputStream(inputInfo))
    {
      throw HootException("Inputs to tag-values must be streamable.");
    }

    LOG_DEBUG("Reading: " << inputInfo << "...");

    reader->open(inputInfo);
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

    finalText = _printJSON("osm", result);
  }

  return finalText;
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

void TagInfo::_parseElement(const ElementPtr& e, TagInfoHash& result)
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

QString TagInfo::_printJSON(const QString& lName, TagInfoHash& data)
{
  QStringList attrKey = data.keys();

  // Skip empty layers
  if (attrKey.count() == 0)
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

  //A bit hackish to handle the situation wher the last key enountered isn't in the specified
  //keys list and avoid an unneeded trailing comma.
  result.replace("        ],\n      }", "        ]\n      }");

  return result;
}

}
