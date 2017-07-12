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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "AttributeCount.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/OgrReader.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/elements/ElementIterator.h>

namespace hoot
{

AttributeCount::AttributeCount() {}


QString AttributeCount::Count(QString input)
{
  QString finalText;
  int maxAttributes = ConfigOptions().getAttributeCountValues();

  OgrReader reader;
  reader.setTranslationFile(QString(getenv("HOOT_HOME")) + "/translations/quick.js");

  QStringList layers;
  if (input.contains(";"))
  {
    QStringList list = input.split(";");
    input = list.at(0);
    layers.append(list.at(1));
  }
  else
  {
    layers = reader.getFilteredLayerNames(input);
  }

  if (layers.size() == 0)
  {
    LOG_WARN("Could not find any valid layers to read from in " + input + ".");
  }

  for (int i = 0; i < layers.size(); i++)
  {
    AttributeCountHash result;

    LOG_DEBUG("Reading: " + input + " " + layers[i]);

    boost::shared_ptr<ElementIterator> iterator(reader.createIterator(input, layers[i]));

    while(iterator->hasNext())
    {
      boost::shared_ptr<Element> e = iterator->next();

      //        // Interesting problem: If there are no elements in the file, e == 0
      //        // Need to look at the ElementIterator.cpp file to fix this.
      //        if (e == 0)
      //        {
      //          LOG_WARN("No features in: " + input + " " + layer);
      //          break;
      //        }

      Tags t = e->getTags();
      for (Tags::const_iterator it = e->getTags().begin(); it != e->getTags().end(); ++it)
      {
        if (it.value() == "") // Drop empty values
        {
          continue;
        }

        // Drop Hoot metadata tags
        if (it.key() == "source:ingest:datetime")
        {
          continue;
        }

        // The default is 30 values
        if (result.value(it.key()).size() < maxAttributes )
        {
          result[it.key()][it.value()]++;
        }
      }
    } // End Layer

    QString tmpText = _printJSON(layers[i], result);

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

  } // End layer list

  return finalText;
} // End AttributeCount


QString AttributeCount::_printJSON(QString lName, AttributeCountHash& data)
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
  result += QString("    \"%1\":{\n").arg(lName);

  for (int i=0; i < attrKey.count(); i++)
  {
    QStringList valKey = data[attrKey[i]].keys();

    int maxValues = valKey.count();
    valKey.sort();

    result += QString("      \"%1\":[\n").arg(attrKey[i]);

    for (int j=0; j < maxValues; j++)
    {
      QString tmpVal(valKey[j]);

      // Escape carrage returns
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

    } // End values

    if (i != (attrKey.count() - 1))
    {
      result += "        ],\n";
    }
    else
    {
      // No comma after bracket
      result += "        ]\n";
    }

  } // End attributes

  // We have no idea if this is the last layer so no comma on the end
  result += "      }";

  return result;

} // End printJSON

}
