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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef MERGECREATOR_H
#define MERGECREATOR_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/MatchSet.h>

// Standard
#include <set>
#include <string>
#include <vector>

namespace hoot
{
using namespace std;

class Match;
class Merger;

class MergerCreator
{
public:
  class Description
  {
  public:
    Description() : experimental() {}
    Description(string className, QString description, bool experimental)
    {
      this->className = className;
      this->experimental = experimental;
      this->description = description;
    }

    QString toString() const
    {
      return QString("%1 %2").arg(QString::fromStdString(className)).
        arg(experimental ? "(experimental)" : "");
    }

    bool experimental;
    string className;
    QString description;
  };

  static string className() { return "hoot::MergerCreator"; }

  virtual ~MergerCreator() {}

  /**
   * Either creates a one or more merges that can merge all the matches in the provided set, or
   * does nothing.
   *
   * @param matches Set of matches that should evaluated to look for mergers.
   * @param mergers Vector of mergers that will contain the new mergers created. Newly created
   *  mergers should only be added and no elements in the vector should be modified.
   * @return Returns true if one or more mergers were created and added to the provided mergers
   *  vector.
   */
  virtual bool createMergers(const MatchSet& matches, vector<Merger*>& mergers) const = 0;

  /**
   * Generally this just returns the class name of this creator. However, creators that take
   * arguments to specify scripts such as the ScriptMatchCreator may return multiple results.
   */
  virtual vector<Description> getAllCreators() const = 0;

  /**
   * Returns true if m1 and m2 are conflicting. If the MergerCreator has no information on the two
   * input matches then false is returned.
   */
  virtual bool isConflicting(const ConstOsmMapPtr& map, const Match* m1, const Match* m2) const = 0;

  virtual void setArguments(QStringList /*args*/) { throw IllegalArgumentException(); }
};

}

#endif // MERGECREATOR_H
