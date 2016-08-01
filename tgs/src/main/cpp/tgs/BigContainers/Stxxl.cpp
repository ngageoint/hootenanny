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
#include "Stxxl.h"

// Qt
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QString>

// Standard
#include <iostream>

// STXXL
#include <stxxl/mng>

// tgs
#include <tgs/TgsException.h>

namespace Tgs
{

shared_ptr<Stxxl> Stxxl::_theInstance;

Stxxl::Stxxl()
{
  _init();
}

Stxxl::~Stxxl()
{
}

Stxxl& Stxxl::getInstance()
{
  if (!_theInstance.get())
  {
    _theInstance.reset(new Stxxl());
  }
  return *_theInstance;
}

void Stxxl::_init()
{
}

QString Stxxl::_removeComments(QString s)
{
  QStringList l = s.split("\n");
  QStringList result;

  for (int i = 0; i < l.size(); i++)
  {
    if (l[i].startsWith("#") == false)
    {
      result.append(l[i]);
    }
  }

  return result.join("\n");
}

void Stxxl::setConfig(QString configFile)
{
  QFile fp(configFile);

  if (fp.open(QIODevice::ReadOnly) == false)
  {
    throw Tgs::Exception(("Error opening file " + configFile).toStdString());
  }

  QString s = fp.readAll();
  long pid = QCoreApplication::applicationPid();

  s.replace("###", QString::number(pid));

  s = _removeComments(s);

  _configFileTmp.setFileTemplate(QDir::tempPath() + "/StxxlConfigXXXXXX.conf");
  _configFileTmp.setAutoRemove(true);

  if (_configFileTmp.open() == false)
  {
    QString err = QString("Error opening file for writing %1 (%2)").
      arg(_configFileTmp.fileName()).arg(_configFileTmp.errorString());
    throw Tgs::Exception(err.toStdString());
  }

  if (_configFileTmp.write(s.toUtf8()) == 0)
  {
    QString err("Error writing data to %1 (%2)");
    err.arg(_configFileTmp.fileName()).arg(_configFileTmp.errorString());
    throw Tgs::Exception(err.toStdString());
  }

  _configFileTmp.close();

  // initialize the environment variable for loading STXXL configuration. If the environment
  // variable has already been set then overwrite it (that is the 1 at the end).
  setenv("STXXLCFG", _configFileTmp.fileName().toAscii().data(), 1);
}

}
