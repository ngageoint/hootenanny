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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/util/Factory.h>
#include <hoot/rnd/util/NodeDensityPlotter.h>

// Qt
#include <QImage>

namespace hoot
{

class PlotNodeDensityCmd : public BaseCommand
{
public:

  static QString className() { return "PlotNodeDensityCmd"; }

  PlotNodeDensityCmd() = default;

  QString getName() const override { return "plot-node-density"; }
  QString getDescription() const override { return "Creates a node density plot for a map"; }
  QString getType() const override { return "rnd"; }

  int runSimple(QStringList& args) override
  {
    NodeDensityPlotter plotter;

    // initialize to black
    QRgb baseColors = qRgba(0, 0, 0, 255);
    if (args.contains("--base-colors"))
    {
      const int baseColorIndex = args.indexOf("--base-colors");
      const QStringList bs = args[baseColorIndex].split(",");
      if (bs.size() != 4)
      {
        throw HootException("Expected --base-colors to be RGBA and comma delimited.");
      }
      baseColors =
        qRgba(
          plotter.toColorBand(bs[0]), plotter.toColorBand(bs[1]), plotter.toColorBand(bs[2]),
          plotter.toColorBand(bs[3]));
      args.removeAt(baseColorIndex + 1);
      args.removeAt(baseColorIndex);
    }

    std::vector<double> multiplier(4, 255.0);
    multiplier[3] = 0.0;
    if (args.contains("--multiplier"))
    {
      const int multiplierIndex = args.indexOf("--multiplier");
      const QStringList bs = args[4].split(",");
      if (bs.size() != 4)
      {
        throw HootException("Expected --multiplier to be RGBA and comma delimited.");
      }
      multiplier[0] = plotter.toColorPortion(bs[0]);
      multiplier[1] = plotter.toColorPortion(bs[1]);
      multiplier[2] = plotter.toColorPortion(bs[2]);
      multiplier[3] = plotter.toColorPortion(bs[3]);
      args.removeAt(multiplierIndex + 1);
      args.removeAt(multiplierIndex);
    }

    if (args.size() != 3)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw IllegalArgumentException(
        QString("%1 takes three parameters. You provided %2: %3")
          .arg(getName())
          .arg(args.size())
          .arg(args.join(",")));
    }

    bool ok;
    const QString input = args[0];
    const QString output = args[1];
    const double maxSize = args[2].toDouble(&ok);
    if (!ok || maxSize < 1.0)
    {
      throw IllegalArgumentException("Expected a number greater 1.0 for max size.");
    }

    plotter.setBaseColors(baseColors);
    plotter.setColorMultiplier(multiplier);
    plotter.setMaxSize(maxSize);
    plotter.plot(input, output);

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, PlotNodeDensityCmd)

}
