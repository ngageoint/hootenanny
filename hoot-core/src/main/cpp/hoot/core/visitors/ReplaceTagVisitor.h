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
#ifndef REPLACETAGVISITOR_H
#define REPLACETAGVISITOR_H

// hoot
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * @brief The ReplaceTagVisitor class replaces matching tags with the
 * specified replacement tag.
 *
 * This was written for the building outline project: the input data had tag
 * BUILDING=yes, but the output data needed to have tag building=yes
 *
 * Feel free to extend this class. Maybe add a flag for case-insensitive
 * matching. Or support for wildcards.
 */
class ReplaceTagVisitor : public ElementVisitor, public Configurable
{
public:

  /**
   * @brief className - get class name string
   * @return The namespace qualified class name
   */
  static std::string className() { return "hoot::ReplaceTagVisitor"; }

  /**
   * @brief ReplaceTagVisitor - default constructor
   */
  ReplaceTagVisitor();

  /**
   * @brief ReplaceTagVisitor - This constructor lets you specify the tag
   * (k=v) to match, and the tag (k=v) to replace it with.
   * @param matchKey - key to match
   * @param matchValue - value to match
   * @param replaceKey - key to use as a replacement
   * @param replaceValue - value to use as a replacement
   */
  ReplaceTagVisitor(QString matchKey, QString matchValue,
                    QString replaceKey, QString replaceValue);

  /**
   * @brief setMatchTag - Set the tag to match
   * @param k - tag key
   * @param v - tag value
   */
  void setMatchTag(QString k, QString v);

  /**
   * @brief setReplaceTag -Set the tag to use as a replacement
   * @param k - tag key
   * @param v - tag value
   */
  void setReplaceTag(QString k, QString v);

  /**
   * @brief visit - visit an element & perform check and replace
   * @param e - element to visit
   */
  virtual void visit(const std::shared_ptr<Element>& e);

  virtual QString getDescription() const
  { return "Replaces matching tags with the specified replacement tag"; }

  virtual void setConfiguration(const Settings& conf);

  virtual QString getInitStatusMessage() const
  { return "Replacing tags..."; }

  virtual QString getCompletedStatusMessage() const
  {
    return "Replaced " + QString::number(_numAffected) + " element tags";
  }

  virtual std::string getClassName() const { return className(); }

private:

  QString _matchKey;
  QString _matchValue;
  QString _replaceKey;
  QString _replaceValue;
};

}

#endif // REPLACETAGVISITOR_H
