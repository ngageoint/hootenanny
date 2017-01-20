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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/ChangesetDeriver.h>
#include <hoot/core/io/ElementSorter.h>
#include <hoot/core/io/OsmChangesetXmlFileWriter.h>
#include <hoot/core/io/OsmChangesetSqlFileWriter.h>
#include <hoot/core/io/HootApiDb.h>

// Qt
#include <QUrl>

namespace hoot
{

class DeriveChangesetCmd : public BaseCommand
{
public:

  static string className() { return "hoot::DeriveChangesetCmd"; }

  DeriveChangesetCmd() { }

  ~DeriveChangesetCmd()
  {
    _hootApiDb.close();
  }

  virtual QString getName() const { return "derive-changeset"; }

  virtual int runSimple(QStringList args)
  {
    bool isXmlOutput = false;

    QString osmApiDbUrl = "";

    bool writeJobStatus = false;
    QString jobId = "";
    QString hootApiDbUrl = "";

    if (args[2].endsWith(".osc"))
    {
      isXmlOutput = true;
      if (args.size() != 3 && args.size() != 5)
      {
        cout << getHelp() << endl << endl;
        throw HootException(
          QString("%1 with XML output takes three or five parameters.").arg(getName()));
      }
      if (args.size() == 5)
      {
        writeJobStatus = true;
        jobId = args[3];
        hootApiDbUrl = args[4];
      }
    }
    else if (args[2].endsWith(".osc.sql"))
    {
      if (args.size() != 4 && args.size() != 6)
      {
        cout << getHelp() << endl << endl;
        throw HootException(
          QString("%1 with SQL output takes four or six parameters.").arg(getName()));
      }
      osmApiDbUrl = args[3];
      if (args.size() == 6)
      {
        writeJobStatus = true;
        jobId = args[4];
        hootApiDbUrl = args[5];
      }
    }
    else
    {
      throw HootException("Unsupported changeset output format: " + args[2]);
    }

    const QString input1 = args[0];
    const QString input2 = args[1];
    const QString output = args[2];

    LOG_INFO(
      "Deriving changeset for inputs " << input1 << ", " << input2 << " and writing output to " <<
      output << "...");

    //use the same unknown1 status for both so they pass comparison correctly
    OsmMapPtr map1(new OsmMap());
    loadMap(map1, input1, true, Status::Unknown1);

    OsmMapPtr map2(new OsmMap());
    loadMap(map2, input2, true, Status::Unknown1);

    ElementSorterPtr sorted1(new ElementSorter(map1));
    ElementSorterPtr sorted2(new ElementSorter(map2));
    ChangesetDeriverPtr delta(new ChangesetDeriver(sorted1, sorted2));

    OsmChangesetXmlFileWriter xmlWriter;
    if (isXmlOutput)
    {
      xmlWriter.write(output, delta);
    }
    else
    {
      assert(!osmApiDbUrl.isEmpty());
      LOG_DEBUG(osmApiDbUrl);
      OsmChangesetSqlFileWriter(QUrl(osmApiDbUrl)).write(output, delta);
    }

    //write the output file name to the job status detail col for later retrieval
    if (writeJobStatus)
    {
      assert(!hootApiDbUrl.isEmpty() && !jobId.isEmpty());
      LOG_DEBUG(
        "Associating changeset file: " << output << " with services job ID: " << jobId < "...");
      LOG_VARD(hootApiDbUrl);
      _hootApiDb.open(QUrl(hootApiDbUrl));
      QString outputPath = output;
      if (isXmlOutput && xmlWriter.getMultipleChangesetsWritten())
      {
        //The xml writer will break up changesets into multiple files if they exceed the maximum
        //changeset size.  Thus, the path will be changed.  The sql writer does not break up the
        //output into seperate files.

        //We're just going to punt on multiple changeset files for now and make sure the services
        //handle the situation appropriately.  We don't want to have to deal with the potentially
        //large size of multiple changesets worth of data when returning changeset data from the
        //services.
        outputPath = "<multiple files>";
      }
      _hootApiDb.writeJobStatus(jobId, outputPath);
    }

    return 0;
  }

private:

  HootApiDb _hootApiDb;

};

HOOT_FACTORY_REGISTER(Command, DeriveChangesetCmd)

}


