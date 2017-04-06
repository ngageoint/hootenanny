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
#include <hoot/core/io/PbfElementIterator.h>
#include <hoot/core/io/OsmPbfWriter.h>
#include <hoot/core/util/Settings.h>

// Standard
#include <fstream>
#include <limits>

namespace hoot
{

class BigCatCmd : public BaseCommand
{
public:

  static string className() { return "hoot::BigCatCmd"; }

  BigCatCmd() { }

  virtual QString getName() const { return "big-cat"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() < 2)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes at least two parameters.").arg(getName()));
    }

    QString output = args[0];

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
    int relationCount = 0;

    vector<long> minNode(args.size() - 1);
    vector<long> maxNode(args.size() - 1);
    vector<long> minWay(args.size() - 1);
    vector<long> maxWay(args.size() - 1);
    vector<long> minRelation(args.size() - 1);
    vector<long> maxRelation(args.size() - 1);

    for (int i = 1; i < args.size(); i++)
    {
      QString input = args[i];

      minNode[i - 1] = numeric_limits<long>::max();
      maxNode[i - 1] = -numeric_limits<long>::max();
      minWay[i - 1] = numeric_limits<long>::max();
      maxWay[i - 1] = -numeric_limits<long>::max();
      minRelation[i - 1] = numeric_limits<long>::max();
      maxRelation[i - 1] = -numeric_limits<long>::max();

      PbfElementIterator it(input);
      while (it.hasNext())
      {
        boost::shared_ptr<Element> e = it.next();
        boost::shared_ptr<Way> w = dynamic_pointer_cast<Way>(e);
        boost::shared_ptr<Node> n = dynamic_pointer_cast<Node>(e);
        boost::shared_ptr<Relation> r = dynamic_pointer_cast<Relation>(e);
        if (w != 0)
        {
          minWay[i - 1] = min(w->getId(), minWay[i - 1]);
          maxWay[i - 1] = max(w->getId(), maxWay[i - 1]);
        }
        else if (n != 0)
        {
          minNode[i - 1] = min(n->getId(), minNode[i - 1]);
          maxNode[i - 1] = max(n->getId(), maxNode[i - 1]);
        }
        else if (r != 0)
        {
          minRelation[i - 1] = min(r->getId(), minRelation[i - 1]);
          maxRelation[i - 1] = max(r->getId(), maxRelation[i - 1]);
        }
        else
        {
          throw HootException("Unexpected element type.");
        }
      }
    }

    int topNode = -1;
    int topWay = -1;
    int topRelation = -1;
    for (int i = 1; i < args.size(); i++)
    {
      QString input = args[i];

      writer.setIdDelta(topNode - maxNode[i - 1], topWay - maxWay[i - 1],
                        topRelation - maxRelation[i - 1]);

      topNode -= maxNode[i - 1] - minNode[i - 1] + 1;
      topWay -= maxWay[i - 1] - minWay[i - 1] + 1;
      topRelation -= maxRelation[i - 1] - minRelation[i - 1] + 1;

      PbfElementIterator it(input);
      while (it.hasNext())
      {
        boost::shared_ptr<Element> e = it.next();
        boost::shared_ptr<Way> w = dynamic_pointer_cast<Way>(e);
        boost::shared_ptr<Node> n = dynamic_pointer_cast<Node>(e);
        boost::shared_ptr<Relation> r = dynamic_pointer_cast<Relation>(e);
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
        else if (r != 0)
        {
          writer.writePartial(r);
          relationCount++;
        }
        else
        {
          throw HootException("Unexpected element type.");
        }
      }
    }

    writer.finalizePartial();

    LOG_INFO("Done writing file.");

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, BigCatCmd)

}


