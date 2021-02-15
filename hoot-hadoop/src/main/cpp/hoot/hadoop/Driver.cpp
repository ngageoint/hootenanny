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
 */

#include "Driver.h"

// hoot
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/OsmSchemaLoaderFactory.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/schema/OsmSchemaLoader.h>

// pretty pipes
#include <pp/mapreduce/Job.h>

// Qt
#include <QFileInfo>

using namespace std;

namespace hoot
{

Driver::Driver()
{
}

void Driver::_addDefaultJobSettings(pp::Job& job)
{
  QString schemaPath = ConfPath::search("schema.json");
  boost::shared_ptr<OsmSchemaLoader> l =
    OsmSchemaLoaderFactory::getInstance().createLoader(schemaPath);
  OsmSchema tmp;
  LOG_INFO("Loading translation files...");
  l->load(schemaPath, tmp);
  set<QString> deps = l->getDependencies();

  QHash<QString,QString> fileList;

  job.addPlugin(getenv("HOOT_HOME") + string("/lib/libHootJs.so.1"));

  for (set<QString>::iterator it = deps.begin(); it != deps.end(); ++it)
  {
    QString fileName(QFileInfo(*it).fileName());

    if (!fileList.contains(fileName))
    {\
      fileList.insert(fileName,*it);

      job.addFile(it->toStdString());
    }
    else
    {
      throw HootException("Duplicate config files: " + *it + " and " + fileList.value(fileName));
    }
  }

  if (!fileList.contains("dictionary.json"))
  {
    job.addFile(ConfPath::search("dictionary/dictionary.json").toStdString());
  }
  else
  {
    throw HootException("Duplicate config files: " + ConfPath::search("dictionary/dictionary.json") +
                        " and " + fileList.value("dictionary.json"));
  }
}

}
