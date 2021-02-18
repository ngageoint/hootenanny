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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/schema/OsmSchema.h>

namespace hoot
{

class TypeSimilarityCmd : public BaseCommand
{

public:

  static QString className() { return "hoot::TypeSimilarityCmd"; }

  TypeSimilarityCmd() = default;

  virtual QString getName() const override { return "type-similarity"; }

  virtual QString getDescription() const override
  { return "Calculates a similarity score between two type tags"; }

  virtual int runSimple(QStringList& args) override
  {
    if (args.size() != 2)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw IllegalArgumentException(QString("%1 takes two parameters.").arg(getName()));
    }

    const QString tag1 = args[0];
    const QString tag2 = args[1];

    const QString invalidKvpMsg = "is not a valid key/value pair of the form: key=value.";
    if (!Tags::isValidKvp(tag1))
    {
      throw IllegalArgumentException(tag1 + " " + invalidKvpMsg);
    }
    if (!Tags::isValidKvp(tag2))
    {
      throw IllegalArgumentException(tag2 + " " + invalidKvpMsg);
    }

    const QString invalidTypeMsg =
      "does not correspond to a type in the Hootenanny internal schema.";
    if (!OsmSchema::getInstance().isTypeKey(tag1.split("=")[0]))
    {
      throw IllegalArgumentException(tag1 + " " + invalidTypeMsg);
    }
    if (!OsmSchema::getInstance().isTypeKey(tag2.split("=")[0]))
    {
      throw IllegalArgumentException(tag2 + " " + invalidTypeMsg);
    }

    std::cout << "Type similarity score (0.0 to 1.0) for " << tag1 << " and " << tag2 << ": " <<
                 OsmSchema::getInstance().score(tag1, tag2) << std::endl;

    return 0;
  }

};

HOOT_FACTORY_REGISTER(Command, TypeSimilarityCmd)

}
