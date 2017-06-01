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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef SCRIPTSTATSCOMPOSER_H
#define SCRIPTSTATSCOMPOSER_H

#include <hoot/core/conflate/StatsComposer.h>
#include <hoot/core/util/NotImplementedException.h>

#include <hoot/js/PluginContext.h>

namespace hoot
{

/**
 * @sa ScriptStatsComposer
 */
class ScriptStatsComposer : public StatsComposer
{
public:

  static std::string className() { return "hoot::ScriptStatsComposer"; }

  ScriptStatsComposer();

  virtual ~ScriptStatsComposer();

  virtual void initialize(const QString& scriptPath, const QString& format, const QString& outputPath,
                          const QVariantMap& args);

  virtual QString compose(QList<QList<SingleStat> >& stats, const QStringList& names);

private:
  boost::shared_ptr<PluginContext> _script;
  QString _format;
  QString _outputPath;
  QString _path;
  QVariantMap _extraArgs;
  int     *_tokenIndex;
  QString _insertionText1;
  QString _insertionText2;
  QString _insertionText3;
  QString _dataTable1;
  double  _grpMax;

  bool _isTokenMatch(QList< QList<SingleStat> >& stats, QString token);
  void _insertValues(QList< QList<SingleStat> >& stats, QString token, QString tokenName);
};

} // end namespace

#endif // STATSCOMPOSER_H
