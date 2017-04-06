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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/PartialOsmMapReader.h>
#include <hoot/core/io/WordCount.h>
#include <hoot/core/io/WordCountWriter.h>

// Standard
#include <iomanip>
#include <map>

namespace hoot
{

using namespace std;

bool greaterThanWordCount(const WordCount& left, const WordCount& right)
{
  return left.count > right.count;
}

class PrintNamesCmd : public BaseCommand
{
public:
  static string className() { return "hoot::PrintNamesCmd"; }

  class NameCountVisitor : public ElementVisitor
  {
  public:
    NameCountVisitor(std::map<QString, int>& counts) : _counts(counts)
    {
      _nonWord.setPattern("[^\\w]");
    }

    virtual void visit(const ConstElementPtr& e)
    {
      QStringList names = e->getTags().getNames();

      for (int i = 0; i < names.size(); i++)
      {
        QStringList words = names[i].split(" ");
        for (int i = 0; i < words.size(); i++)
        {
          // See wikipedia for a description of what this does, but we're trying to group words
          // that are effectively the same together in the same hash bin.
          // http://en.wikipedia.org/wiki/Unicode_equivalence
          QString word = words[i].toLower().normalized(QString::NormalizationForm_C);
          // remove all the non-word characters (e.g. "(", "-", etc.)
          QString noSymbols = word;
          noSymbols.replace(_nonWord, "");
          // if there is still a word left then use the word w/o symbols.
          // E.g. don't remove "&" if it is all by itself.
          if (noSymbols.isEmpty() == false)
          {
            word = noSymbols;
          }
          // ignore any empty words. This may happen when there are multiple consecutive spaces.
          if (word.isEmpty() == false)
          {
            _counts[word]++;
          }
        }
      }
    }

  private:
    map<QString, int>& _counts;
    QRegExp _nonWord;
  };

  PrintNamesCmd() {}

  virtual QString getName() const { return "names"; }

  int runSimple(QStringList args)
  {
    if (args.size() == 0)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes one or more parameters.").arg(getName()));
    }

    bool writeFile = false;
    QString output;
    if (args.size() >= 3 && args[args.size() - 2] == "--write")
    {
      writeFile = true;
      output = args[args.size() - 1];
    }

    // adding the source datetime just makes things really slow.
    conf().set(ConfigOptions().getReaderAddSourceDatetimeKey(), false);

    boost::shared_ptr<OsmMap> map(new OsmMap());

    std::map<QString, int> counts;
    NameCountVisitor v(counts);
    map->visitRo(v);

    int inputCount = writeFile ? args.size() - 2 : args.size();
    for (int i = 0; i < inputCount; i++)
    {
      if (OsmMapReaderFactory::getInstance().hasPartialReader(args[i]))
      {
        boost::shared_ptr<OsmMapReader> reader = OsmMapReaderFactory::getInstance().createReader(args[i]);
        boost::shared_ptr<PartialOsmMapReader> pomr = dynamic_pointer_cast<PartialOsmMapReader>(reader);

        pomr->open(args[i]);

        while (pomr->hasMoreElements())
        {
          ElementPtr e = pomr->readNextElement();
          v.visit(e);
        }
      }
      else
      {
        boost::shared_ptr<OsmMap> map(new OsmMap());
        loadMap(map, args[i], false, Status::Unknown1);
        map->visitRo(v);
      }
    }

    long total = 0;
    QVector<WordCount> wc;
    wc.reserve(counts.size());
    for (std::map<QString, int>::iterator it = counts.begin(); it != counts.end(); it++)
    {
      wc.push_back(WordCount(it->first, it->second));
      total += it->second;
    }

    if (writeFile)
    {
      WordCountWriter writer(10 * 1000 * 1000);
      writer.write(output, wc);
    }
    else
    {
      cout << "Total word count: " << total << endl;

      sort(wc.begin(), wc.end(), greaterThanWordCount);

      for (int i = 0; i < wc.size(); i++)
      {
        double p = (double)wc[i].count / (double)total;
        cout << setprecision(2);
        cout << wc[i].count << " (" << p << ") : " << wc[i].word.toUtf8().data() << endl;
      }
    }

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, PrintNamesCmd)

}
