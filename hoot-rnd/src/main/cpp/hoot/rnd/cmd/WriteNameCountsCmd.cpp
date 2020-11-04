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
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/rnd/io/WordCount.h>
#include <hoot/rnd/io/WordCountWriter.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/schema/TagDistribution.h>
#include <hoot/core/util/StringUtils.h>

// Standard
#include <map>

// Qt
#include <QElapsedTimer>

namespace hoot
{

using namespace std;

bool greaterThanWordCount(const WordCount& left, const WordCount& right)
{
  return left.count > right.count;
}

class WriteNameCountsCmd : public BaseCommand
{
public:

  static string className() { return "hoot::WriteNameCountsCmd"; }

  WriteNameCountsCmd() = default;

  virtual QString getName() const override { return "write-name-counts"; }

  virtual QString getDescription() const override
  { return "Writes name tag counts for a map"; }

  virtual QString getType() const { return "rnd"; }

  virtual int runSimple(QStringList& args) override
  {
    QElapsedTimer timer;
    timer.start();

    if (args.size() < 2)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes at least two parameters.").arg(getName()));
    }

    // adding the source datetime just makes things really slow.
    conf().set(ConfigOptions().getReaderAddSourceDatetimeKey(), false);

    const QStringList inputs = args[0].split(";");
    const QString output = args[1];

    TagDistribution tagDist;
    tagDist.setTagKeys(Tags::getNameKeys());
    tagDist.setTokenize(true);

    const std::map<QString, int> nameCounts = tagDist.getTagCounts(inputs);

    long total = 0;
    QVector<WordCount> wc;
    wc.reserve(nameCounts.size());
    for (std::map<QString, int>::const_iterator it = nameCounts.begin(); it != nameCounts.end();
         ++it)
    {
      wc.push_back(WordCount(it->first, it->second));
      total += it->second;
    }

    WordCountWriter writer(10 * 1000 * 1000);
    writer.write(output, wc);

    cout << "Total word count: " << total << endl;

    LOG_STATUS(
      "Names counted in " << StringUtils::millisecondsToDhms(timer.elapsed()) << " total.");

    return 0;
  }
};

// not currently in use
//HOOT_FACTORY_REGISTER(Command, WriteNameCountsCmd)

}
