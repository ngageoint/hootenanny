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

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/ops/MergeNearbyNodes.h>
#include <hoot/core/io/OgrReader.h>
#include <hoot/core/io/OsmPbfWriter.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/elements/ElementIterator.h>

// Standard
#include <fstream>

namespace hoot
{

class BigOgr2OsmCmd : public BaseCommand
{
public:

  static string className() { return "hoot::BigOgr2OsmCmd"; }

  BigOgr2OsmCmd() { }

  virtual QString getName() const { return "big-ogr2osm"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() < 3)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes at least three parameters.").arg(getName()));
    }

    QString translation = args[0];
    QString output = args[1];

    if (output.endsWith(".osm.pbf") == false)
    {
      throw HootException("The output must be .osm.pbf.");
    }

    OsmPbfWriter writer;
    ofstream fp;
    fp.open(output.toUtf8().data(), ios::out | ios::binary);
    if (fp.is_open() == false)
    {
      throw HootException("Error opening " + output + " for writing.");
    }
    writer.intializePartial(&fp);

    int nodeCount = 0;
    int wayCount = 0;

    OgrReader reader;
    reader.setTranslationFile(translation);

    for (int i = 2; i < args.size(); i++)
    {
      QString input = args[i];

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
          // Added this so the output is roughly the same as Ogr2OsmCmp.cpp
          LOG_INFO("Reading: " + input + " " + layers[i]);

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

              boost::shared_ptr<Way> w = dynamic_pointer_cast<Way>(e);
              boost::shared_ptr<Node> n = dynamic_pointer_cast<Node>(e);

              if (w != 0)
              {
                  writer.writePartial(w);
                  wayCount++;
              }
              else if (n != 0)
              {
                  writer.writePartial(n);
                  nodeCount++;
              }
              else
              {
                  throw HootException("Unexpected element type.");
              }
          }
      }
    }

    writer.finalizePartial();

    LOG_INFO("Done writing file.");

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, BigOgr2OsmCmd)

}

