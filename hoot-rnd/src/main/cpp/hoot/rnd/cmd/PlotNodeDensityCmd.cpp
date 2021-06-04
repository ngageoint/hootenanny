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

  static QString className() { return "hoot::PlotNodeDensityCmd"; }

  PlotNodeDensityCmd() = default;

  QString getName() const override { return "plot-node-density"; }
  QString getDescription() const override { return "Creates a node density plot for a map"; }
  QString getType() const { return "rnd"; }

  int runSimple(QStringList& args) override
  {
    if (args.size() < 3 || args.size() > 5)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes three to five parameters.").arg(getName()));
    }

    bool ok;
    const QString input = args[0];
    const QString output = args[1];
    const double maxSize = args[2].toDouble(&ok);
    if (!ok || maxSize < 1.0)
    {
      throw IllegalArgumentException("Expected a number greater 1.0 for max size.");
    }

    NodeDensityPlotter plotter;

    // initialize to black
    QRgb baseColors = qRgba(0, 0, 0, 255);
    if (args.size() >= 4)
    {
      QStringList bs = args[3].split(",");
      if (bs.size() != 4)
      {
        throw HootException("Expected base-colors to be RGBA and comma delimited.");
      }
      baseColors =
        qRgba(
          plotter.toColorBand(bs[0]), plotter.toColorBand(bs[1]), plotter.toColorBand(bs[2]),
          plotter.toColorBand(bs[3]));
    }
    std::vector<double> colorMultiplier(4, 255.0);
    colorMultiplier[3] = 0.0;
    if (args.size() >= 5)
    {
      QStringList bs = args[4].split(",");
      if (bs.size() != 4)
      {
        throw HootException("Expected base-colors to be RGBA and comma delimited.");
      }
      colorMultiplier[0] = plotter.toColorPortion(bs[0]);
      colorMultiplier[1] = plotter.toColorPortion(bs[1]);
      colorMultiplier[2] = plotter.toColorPortion(bs[2]);
      colorMultiplier[3] = plotter.toColorPortion(bs[3]);
    }

    plotter.setBaseColors(baseColors);
    plotter.setColorMultiplier(colorMultiplier);
    plotter.setMaxSize(maxSize);
    plotter.plot(input, output);

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, PlotNodeDensityCmd)

}
