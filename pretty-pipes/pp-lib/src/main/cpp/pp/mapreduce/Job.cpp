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

#include "Job.h"

// Qt
#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QDomText>
#include <QFileInfo>
#include <QProcess>
#include <QSet>
#include <QTemporaryFile>

// Pretty Pipes
#include <pp/Exception.h>
#include <pp/Factory.h>
#include <pp/Hdfs.h>
#include <pp/RecordReader.h>
#include <pp/mapreduce/Mapper.h>
#include <pp/mapreduce/Reducer.h>
#include <pp/mapreduce/RecordWriter.h>

// Standard
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
using namespace std;

#include "LocalJobRunner.h"

namespace pp
{

string Job::_defaultTracker;

Job::Job()
{
  _conf.set("hadoop.pipes.java.recordreader", "true");
  _conf.set("hadoop.pipes.java.recordwriter", "true");
  _conf.set("hadoop.pipes.executable", "/usr/pretty-pipes/PrettyPipes");
  if (getenv("CLASSPATH") != NULL)
  {
    _conf.set("CLASSPATH", getenv("CLASSPATH"));
  }
  _verbose = false;

  if (_defaultTracker.empty() == false)
  {
    setJobTracker(_defaultTracker);
  }

  // the java portion of our task needs very little RAM.
  //addChildJavaOpts("-Xmx512m");
}

void Job::addChildJavaOpts(const std::string& opts)
{
  _opts.append(QString::fromStdString(opts));
  _conf.set("mapred.child.java.opts", _opts.join(" ").toStdString());
}

void Job::addLibraryDirs(const QStringList& dirs)
{
  for (int i = 0; i < dirs.size(); i++)
  {
    addLibraryDir(dirs[i].toStdString());
  }
}

QString Job::_getFiles()
{
  QStringList result;
  QStringList libFilter;
  libFilter << "lib*.so*";

  for (size_t i = 0; i < _libraryDir.size(); i++)
  {
    QDir d(QString::fromStdString(_libraryDir[i]));
    if (d.exists())
    {
      QFileInfoList fil = d.entryInfoList(libFilter, QDir::Files, QDir::Name);
      for (int i = 0; i < fil.size(); i++)
      {
        result << fil.at(i).absoluteFilePath();
      }
    }
  }

  for (size_t i = 0; i < _plugins.size(); i++)
  {
    result << QString::fromStdString(_plugins[i]);
  }

  for (size_t i = 0; i < _files.size(); i++)
  {
    result << QString::fromStdString(_files[i]);
  }

  return QStringList(result.toSet().toList()).join(",");
}

QString Job::_getPpHome()
{
  const char* ppHome = getenv("PRETTY_PIPES_HOME");
  if (ppHome == NULL)
  {
    throw Exception("PRETTY_PIPES_HOME is not set.");
  }
  return QString(ppHome);
}

void Job::run()
{
  if (getJobTracker() == "local")
  {
    _runLocal();
  }
  else
  {
    _runHadoop();
  }
}

void Job::_runHadoop()
{
  QTemporaryFile tmp;
  if (tmp.open() == false)
  {
    throw Exception("Error creating temporary file.");
  }

  QStringList plugins;
  for (size_t i = 0; i < _plugins.size(); i++)
  {
    QFileInfo fi(QString::fromStdString(_plugins[i]));
    plugins << fi.fileName();
  }
  _conf.set(PP_PLUGINS, plugins.join(",").toStdString());

  if (_conf.hasKey(PP_REDUCER) == false || _conf.get(PP_REDUCER) == "")
  {
    _conf.set("mapred.reducer.class", "");
  }

  _writeConfiguration(tmp);
  tmp.flush();
  tmp.close();

  _uploadBinary();

  Hdfs fs;
  if (fs.exists(_output))
  {
    fs.deletePath(_output, true);
  }

  // compose command line
  QStringList args;
  args << "hadoop";
  args << "pipes";
  if (_getFiles().size() > 0)
  {
    args << "-files" << _getFiles();
  }
  args << "-conf" << tmp.fileName();
  args << "-input" << QString::fromStdString(_input);
  args << "-output" << QString::fromStdString(_output);

  // mark up spaces and double quotes so they're parsed properly.
  for (int i = 0; i < args.size(); i++)
  {
    if (args[i].contains("\"") || args[i].contains(" "))
    {
      args[i] = "\"" + args[i].replace("\"", "\\\"") + "\"";
    }
  }

  if (_verbose)
  {
    cout << args.join(" ").toStdString() << endl << flush;
  }
  char buffer[11];
  FILE* p = popen(args.join(" ").toStdString().data(), "r");
  while (!feof(p))
  {
    size_t s = fread(buffer, 1, 10, p);
    buffer[s] = 0;
    if (_verbose)
    {
      cout << buffer << flush;// << endl;
    }
  }
  int status = pclose(p);
  if (status != 0)
  {
    throw Exception("Error running job.");
  }
}

void Job::_runLocal()
{
  LocalJobRunner ljr(_conf);
  ljr.run();
}

void Job::setInputFormatClass(const std::string& inputFormat)
{
  _conf.set("mapred.jar", (_getPpHome() + "/lib/PrettyPipesLib-0.0.1.jar").toStdString());
  _conf.set("mapred.input.format.class", "pp.InputFormatWrapper");
  _conf.set(PP_INPUT_FORMAT, inputFormat);
}

void Job::setMapperClass(const std::string& mapper)
{
  pp::Factory::getInstance().checkClass<pp::Mapper>(mapper);
  _conf.set(PP_MAPPER, mapper);
}

void Job::setOutput(const std::string& output)
{
  _output = output;
  _conf.set("mapred.output.dir", output);
}

void Job::setRecordReaderClass(const std::string& reader)
{
  pp::Factory::getInstance().checkClass<pp::RecordReader>(reader);
  _conf.set("hadoop.pipes.java.recordreader", "false");
  _conf.set(PP_RECORD_READER, reader);
}

void Job::setRecordWriterClass(const std::string& writer)
{
  pp::Factory::getInstance().checkClass<pp::RecordWriter>(writer);
  _conf.set("hadoop.pipes.java.recordwriter", "false");
  _conf.set(PP_RECORD_WRITER, writer);
}

void Job::setReducerClass(const std::string& reducer)
{
  pp::Factory::getInstance().checkClass<pp::Reducer>(reducer);
  _conf.set(PP_REDUCER, reducer);
}

void Job::_uploadBinary()
{
  string localPath = (_getPpHome() + QString("/bin/PrettyPipes")).toStdString();
  string hdfsDir = "/usr/pretty-pipes/";
  string hdfsPath = hdfsDir + "PrettyPipes";

  QFileInfo f(QString::fromStdString(localPath));
  if (f.exists() == false)
  {
    throw Exception("Unable to find executable: " + f.absoluteFilePath());
  }

  bool copy = false;
  Hdfs fs;
  if (!fs.exists(hdfsPath))
  {
    copy = true;
    if (fs.exists(hdfsDir) == false)
    {
      fs.mkdirs(hdfsDir);
    }
  }
  else
  {
    FileStatus status = fs.getFileStatus(hdfsPath);
    if (f.lastModified().toTime_t() > status.getModificationTime() ||
        f.size() != status.getLen())
    {
      copy = true;
    }
  }

  if (copy)
  {
    if (_verbose)
    {
      cout << "*********" << endl;
      cout << "* Uploading new pretty pipes executable." << endl;
      cout << "*********" << endl;
      cout.flush();
    }
    fs.copyFromLocal(localPath, hdfsPath);
  }
}

void Job::_writeConfiguration(QFile& f)
{
  QDomDocument doc;

  QDomNode header(doc.createProcessingInstruction("xml", "version=\"1.0\""));
  doc.insertAfter(header, doc.firstChild());

  QDomElement root = doc.createElement("configuration");
  doc.appendChild(root);

  const map<string, string>& m = _conf.getMap();
  for (map<string, string>::const_iterator it = m.begin(); it != m.end(); ++it)
  {
    QDomElement property = doc.createElement("property");
    root.appendChild(property);
    QDomElement name = doc.createElement("name");
    property.appendChild(name);
    QDomText nameValue = doc.createTextNode(QString::fromStdString(it->first));
    name.appendChild(nameValue);
    QDomElement value = doc.createElement("value");
    property.appendChild(value);
    QDomText valueValue = doc.createTextNode(QString::fromStdString(it->second));
    value.appendChild(valueValue);
  }

  if (_verbose)
  {
    cout << doc.toByteArray().data() << endl;
  }
  if (f.write(doc.toByteArray()) == false)
  {
    throw Exception("Error writing configuration to temporary file.");
  }
}


}
