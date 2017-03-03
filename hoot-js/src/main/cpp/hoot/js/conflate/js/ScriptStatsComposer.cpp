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
#include "ScriptStatsComposer.h"

// c++ includes
#include <limits>

// Qt
#include <QStringList>

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/js/OsmMapJs.h>
#include <hoot/js/elements/ElementJs.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(StatsComposer, ScriptStatsComposer)

ScriptStatsComposer::ScriptStatsComposer()
{
}

ScriptStatsComposer::~ScriptStatsComposer()
{
}

void ScriptStatsComposer::initialize(const QString& scriptPath, const QString& format, const QString& outputPath,
                                     const QVariantMap& args)
{
  cout << "Inside ScriptStatsComposer::initialize" << endl;

  // store params
  _outputPath = outputPath;
  _format = format;
  _tokenIndex = new int[3];
  _extraArgs = args;

  // read in script
  _path = ConfPath::search(scriptPath, "rules");
  _script.reset(new PluginContext());

}

bool ScriptStatsComposer::_isTokenMatch(QList< QList<SingleStat> >& stats, QString token)
{
  bool foundMatches = true;
  for(int ti=0; ti<3; ti++)
  {
    _tokenIndex[ti] = -1;
    bool foundMatch = false;
    for(int i=0; i<stats[ti].size() && !foundMatch; i++)
    {
      if(token == stats[ti][i].name)
      {
        foundMatch = true;
        _tokenIndex[ti] = i;
      }
    }
    foundMatches = (foundMatches && foundMatch);
  }
  return foundMatches;
}

void ScriptStatsComposer::_insertValues(QList< QList<SingleStat> >& stats, QString token, QString tokenName)
{
  double value1, value2, value3;
  double divisor = 1.0;
  double multiplier = 1.0;

  //if(isnan(stats[0][_tokenIndex].value)) value1 = 0;
  //else
  value1 = stats[0][_tokenIndex[0]].value;
  if(token=="distanceTokens1") divisor=1000.0; // meters to km
  else if(token=="areaTokens1") divisor=1000000.0; // meters^2 to km^2
  else if(token=="tagPercentageTokens1") multiplier=100.0; // decimal to percent
  value1 = value1 * multiplier / divisor;
  _grpMax = max(_grpMax, value1);
  //QString tmpStr = stats[0][_tokenIndex].name;
  //_insertionText1 += ( tmpStr.replace(QString(" "),QString("_")) + "\t" + QString::number(value) + "\n" );
  _insertionText1 += ( "\"" + tokenName + "\"\t" + QString::number(value1) + "\n" );

  //if(isnan(stats[1][_tokenIndex].value)) value2 = 0;
  //else
  value2 = stats[1][_tokenIndex[1]].value;
  value2 = value2 * multiplier / divisor;
  _grpMax = max(_grpMax, value2);
  //tmpStr = stats[1][_tokenIndex].name;
  _insertionText2 += ( "\"" + tokenName + "\"\t" + QString::number(value2) + "\n" );

  //if(isnan(stats[2][_tokenIndex].value)) value3 = 0;
  //else
  value3 = stats[2][_tokenIndex[2]].value;
  value3 = value3 * multiplier / divisor;
  _grpMax = max(_grpMax, value3);
  //tmpStr = stats[2][_tokenIndex].name;
  _insertionText3 += ( "\"" + tokenName + "\"\t" + QString::number(value3) + "\n" );

  _dataTable1 += ( "|"+tokenName+"|"+QString::number(value1)+"|"+QString::number(value2)+"|"+
                  QString::number(value3) + "\n" );
}

QString ScriptStatsComposer::compose(QList< QList<SingleStat> >& stats, const QStringList& names)
{
  // NOTE: Some stuff in here is optimized for using GNU PLOT, but you should be able to use
  //   other drawing packages by using the stats passed into the JS file.

  // vars
  HandleScope handleScope;
  Context::Scope context_scope(_script->getContext());
  QString report = "";

  // get exports from script
  Handle<Object> exports = _script->loadScript(_path, "plugin");
  //cout << "exports = " << exports << endl;

  QStringList tmpText = names[0].split("/");
  QString input1 = tmpText.last();
  tmpText = names[1].split("/");
  QString input2 = tmpText.last();

  ///////////////////////////////////////////////////////////////
  // setup histogram-like tables (3 bar graphs per features:
  //   input1, input2, conflated output)
  //   - uses 1st-3rd QLists of stats object
  ///////////////////////////////////////////////////////////////
  QStringList histTokens;
  histTokens << "countTokens1" << "countTokens2" << "countTokens3" << "percentageTokens1" <<
                "distanceTokens1" << "areaTokens1" << "tagCountTokens1" << "tagCountTokens2" <<
                "tagPercentageTokens1" << "poiTokens" << "uniqueTokens";

  QVariantMap histTables;
  QVariantMap dataTables;

  for(int p=0; p<histTokens.size(); p++)
  {
    std::string tokenString = histTokens[p].toStdString();
    //cout << "token String = " << tokenString << endl;
    vector<QString> tokenList = toCpp< vector<QString> >( exports->Get(String::New(tokenString.c_str())));
    _insertionText1 = "\"Input 1\"\n";
    _insertionText2 = "\"Input 2\"\n";
    _insertionText3 = "\"Output\"\n";
    _dataTable1 = "";
    _grpMax = 0;
    for(unsigned int t=0; t<tokenList.size(); t++)
    {
      // with current token
      QString tokentmp= tokenList[t].toLocal8Bit().constData();
      QStringList tokenStrings = tokentmp.split(";");
      QString token = tokenStrings[0];
      QString tokenName = token;
      if(tokenStrings.size()==2) tokenName = tokenStrings[1];
      // find a match found, and if a match, then add stats information to insertion text
      if( _isTokenMatch(stats, token) ) _insertValues(stats, histTokens[p], tokenName);
    }
    // grow _grpMax by 10% to give space near top
    //int oldMax = (int)_grpMax;
    _grpMax = (double)(_grpMax*1.2);
    // if didn't move up > max, then add 1 integer to be safe
    //if((int)_grpMax == oldMax) _grpMax += 1;
    if(_grpMax == 0) _grpMax = 1;

    // create a map of the tables as QStrings
    histTables[histTokens[p]] =
            QString("set yrange [0:") + QString::number(_grpMax) + QString("]\n") +
            QString("plot '-' using 2:xtic(1) ti col, '' u 2 ti col, '' u 2 ti col\n") +
            _insertionText1 + "e\n" +
            _insertionText2 + "e\n" +
            _insertionText3 + "e\n";

    // make asciidoc tables for each bar graph
    dataTables[histTokens[p]] =
            QString("[width=\"90%\",options=\"header\"]\n") +
            QString("|====================================\n") +
           // QString("| |")+input1.left(19)+QString("|")+input2.left(19)+QString("|Conflated file\n") +
            QString("| |Input 1|Input 2|Output\n") +
            _dataTable1 + "\n" +
            QString("|====================================\n");

  }

  ///////////////////////////////////////////////////////////////
  // load text body
  ///////////////////////////////////////////////////////////////
  Handle<Function> textbodyFunc = Handle<Function>::Cast( exports->Get(String::New("textBody")) );
  Handle<Value> jsArgs[7];

  int argc = 0;
  QVariantMap input1Map;
  QVariantMap input2Map;
  QVariantMap outputMap;
  for(int i=0; i<stats[0].size(); i++)
  {
    input1Map[stats[0][i].name] = stats[0][i].value;
    input2Map[stats[1][i].name] = stats[1][i].value;
    outputMap[stats[2][i].name] = stats[2][i].value;
  }
  QVariantMap stats4;
  for(int i=0; i<stats[3].size(); i++)
  {
    stats4[stats[3][i].name] = stats[3][i].value;
  }

  jsArgs[argc++] = toV8(input1Map);
  jsArgs[argc++] = toV8(input2Map);
  jsArgs[argc++] = toV8(outputMap);
  jsArgs[argc++] = toV8(histTables);
  jsArgs[argc++] = toV8(stats4);
  jsArgs[argc++] = toV8(dataTables);
  jsArgs[argc++] = toV8(_extraArgs);

  Handle<Value> tbResultHandle = textbodyFunc->Call(exports, argc, jsArgs);
  v8::String::Utf8Value tbParam(tbResultHandle->ToString());
  report = QString::fromStdString(string(*tbParam));

  delete(_tokenIndex);

  return report;
}


} // end hoot namespace
