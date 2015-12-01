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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

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
 * @copyright Copyright (C) 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/io/OgrReader.h>
#include <hoot/core/io/OgrWriter.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/io/schema/StrictChecking.h>

#include <boost/shared_ptr.hpp>

namespace hoot
{

class ExportCmd : public BaseCommand
{
public:

  static string className() { return "hoot::ExportCmd"; }

  ExportCmd() { }

  virtual QString getHelp() const
  {

    const QString defaultCacheCapacity = QString::number(OgrWriter::getDefaultCacheCapacity());

    // 80 columns
    //  | <---                                                                      ---> |
    return getName() + " (translation) (output) (inputURL) [cacheCapacity]\n"
        "  * translation - JavaScript file name.\n"
        "  * output - Output file name. The format is determined by extension. \n"
        "      FileGDB (*.gdb) and Shapefile (*.shp) have been tested but other\n"
        "      OGR-compatible formats will likely work.\n"
        "  * inputURL - An identifier for a PostGIS database. \n"
        "      (e.g., PG:\"dbname='databasename' host='addr' port='5432' user='x' \n"
        "      password='y'\")\n"
        "  * cacheCapacity - OPTIONAL - number of cache entries for *each* of the three \n"
        "      element types (nodes, ways, relations). If not specified, will use \n"
        "      default capacity value of " + defaultCacheCapacity + " elements per type \n";
  }

  virtual QString getName() const { return "export"; }

  static QString opsKey() { return "export.ops"; }

  virtual int runSimple(QStringList args)
  {
    if ( (args.size() < 3) || (args.size() > 4) )
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes either three or four parameters.").arg(getName()));
    }

    int a = 0;
    QString translation = args[a++];
    QString output = args[a++];
    QString input = args[a++];
    unsigned long elementCacheSize = 0;

    if ( args.size() == 4 )
    {
      elementCacheSize = args[a++].toLong();
    }

    LOG_DEBUG("Input: " << input );

    OgrReader inputReader;
    if ( inputReader.isReasonablePath(input) == false )
    {
      throw HootException(QString("Input URL %l is not valid.").arg(input));
    }
    inputReader.open(input);
    LOG_DEBUG("Input database opened successfully!");

    // Set up the writer to take TDS schema elements and write them to an OGR format output
    OgrWriter ogrWriter;
    if ( elementCacheSize > 0 )
    {
      ogrWriter.setCacheCapacity(elementCacheSize);
    }
    ogrWriter.setScriptPath(translation);
    ogrWriter.open(output);

    unsigned long long elementsRead(0);
    long lastPercentComplete = 0;
    while ( inputReader.hasMoreElements() == true )
    {
      elementsRead++;

      //LOG_INFO("Element #" << elementsRead <<": write start");
      ogrWriter.writeElement(inputReader, false);
      //LOG_DEBUG("Element #" << elementsRead <<": write complete!");

      long percentComplete = inputReader.streamGetProgress().getPercentComplete();
      if ( percentComplete != lastPercentComplete)
      {
        LOG_INFO("Export progress: " << percentComplete << "% complete");
        lastPercentComplete = percentComplete;
      }
    }

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, ExportCmd)

}

