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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef BASECOMMAND_H
#define BASECOMMAND_H

// geos
#include <geos/geom/Envelope.h>

// Hoot
#include <hoot/core/cmd/Command.h>
#include <hoot/core/elements/Status.h>

// Qt
#include <QStringList>

namespace hoot
{

class BaseCommand : public Command
{
public:

  BaseCommand() = default;
  virtual ~BaseCommand() = default;

  QString getHelp() const override;

  /**
   * Parses a comma delimited envelope in the form minx,miny,maxx,maxy.
   */
  geos::geom::Envelope parseEnvelope(QString envStr) const;

  /**
   * This method will pull out common arguments (e.g. --conf), convert the args to a QStringList
   * and pass it to runSimple.
   */
  int run(char* argv[], int argc) override;

  /**
   * This is the preferred method to override.
   */
  virtual int runSimple(QStringList& args) = 0;

  QStringList toQStringList(char* argv[], int argc) const;

protected:

  // This can be useful for debugging, or in some cases, getting around the fact that ConfigOptions
  // is a Singleton when trying to run successive conflate jobs.
  QStringList _rawArgs;

  virtual QString _getHelpPath() const;

  /*
   * Parses the --recursive parameter used by several commands to parse input directories
   * recursively from args.
   *
   * @param args command line arguments; If --recursive is found in args, it is removed from them.
   * @param paramPresent determines if --recursive was present in args
   * @return the name filters specified as part of --recursive; An empty list is returned if no
   * filtering was specified with "*".
   */
  static QStringList _parseRecursiveInputParameter(QStringList& args, bool& paramPresent);
};

using BaseCommandPtr = std::shared_ptr<BaseCommand>;

}

#endif // BASECOMMAND_H
