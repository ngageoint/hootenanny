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
 * @copyright Copyright (C) 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "TagInfo.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/OgrReader.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/elements/ElementIterator.h>
#include <hoot/core/io/OsmMapReaderFactory.h>

namespace hoot
{

TagInfo::TagInfo(const int tagValuesPerKeyLimit, const QStringList keys, const bool keysOnly,
                 const bool caseSensitive) :
_tagValuesPerKeyLimit(tagValuesPerKeyLimit),
_keys(keys),
_keysOnly(keysOnly),
_caseSensitive(caseSensitive)
{
}

QString TagInfo::getInfo(QString input)
{
  LOG_VARD(input);
  LOG_VART(_tagValuesPerKeyLimit);
  LOG_VART(_keys);
  LOG_VART(_keysOnly);
  LOG_VART(_caseSensitive);
  QString finalText;

  boost::shared_ptr<OsmMapReader> reader =
    OsmMapReaderFactory::getInstance().createReader(
      input, ConfigOptions().getReaderUseDataSourceIds(),
      Status::fromString(ConfigOptions().getReaderSetDefaultStatus()));

  //Using a different code path for the OGR inputs to handle the layer syntax.  There may be
  //a way to combine the two logic paths...not sure, though.
  boost::shared_ptr<OgrReader> ogrReader =
    boost::dynamic_pointer_cast<OgrReader>(reader);
  if (ogrReader.get())
  {
    ogrReader->setTranslationFile(QString(getenv("HOOT_HOME")) + "/translations/quick.js");

    QStringList layers;
    if (input.contains(";"))
    {
      QStringList list = input.split(";");
      input = list.at(0);
      layers.append(list.at(1));
    }
    else
    {
      layers = ogrReader->getFilteredLayerNames(input);
    }

    if (layers.size() == 0)
    {
      LOG_WARN("Could not find any valid layers to read from in " + input + ".");
    }

    for (int i = 0; i < layers.size(); i++)
    {
      TagInfoHash result;

      LOG_DEBUG("Reading: " << input + " " << layers[i] << "...");

      boost::shared_ptr<ElementIterator> iterator(ogrReader->createIterator(input, layers[i]));

      while (iterator->hasNext())
      {
        boost::shared_ptr<Element> e = iterator->next();

        //        // Interesting problem: If there are no elements in the file, e == 0
        //        // Need to look at the ElementIterator.cpp file to fix this.
        //        if (e == 0)
        //        {
        //          LOG_WARN("No features in: " + input + " " + layer);
        //          break;
        //        }

        _parseElement(e, result);
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
    //At this time, the only unstreamable readers are the JSON readers.  If this capability is
    //needed for JSON data, then either those readers can implement PartialOsmMapReader or the
    //needed readed code can be manually added to this class.

    if (!OsmMapReaderFactory::getInstance().hasElementInputStream(input))
    {
      throw HootException("Inputs to tag-values must be streamable.");
    }

    LOG_DEBUG("Reading: " << input << "...");

    reader->open(input);
    boost::shared_ptr<ElementInputStream> streamReader =
      boost::dynamic_pointer_cast<ElementInputStream>(reader);

    TagInfoHash result;
    while (streamReader->hasMoreElements())
    {
      ElementPtr e = streamReader->readNextElement();
      if (e.get())
      {
        LOG_VART(e);
        _parseElement(e, result);
      }
    }
    boost::shared_ptr<PartialOsmMapReader> partialReader =
      boost::dynamic_pointer_cast<PartialOsmMapReader>(reader);
    if (partialReader.get())
    {
      partialReader->finalizePartial();
    }

    finalText = _printJSON("osm", result);
  }

  return finalText;
}

void TagInfo::_parseElement(ElementPtr e, TagInfoHash& result)
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
    if (it.key() == "source:ingest:datetime")
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

QString TagInfo::_printJSON(QString lName, TagInfoHash& data)
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
    const Qt::CaseSensitivity caseSensitivity =
      _caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;
    if (_keys.isEmpty() || _keys.contains(key, caseSensitivity))
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
