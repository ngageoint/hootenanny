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
 * @copyright Copyright (C) 2015, 2017 Maxar (http://www.maxar.com/)
 */

#ifndef JOB_H
#define JOB_H

// Pretty Pipes
#include <pp/Strings.h>
#include <pp/conf/Configuration.h>

// Qt
#include <QFile>
#include <QStringList>

// Standard
#include <vector>


namespace pp
{

class Job
{
public:
  Job();

  void addChildJavaOpts(const std::string& opts);

  void addFile(const std::string& file) { _files.push_back(file); }

  void addLibraryDir(const std::string& dir) { _libraryDir.push_back(dir); }

  /**
   * Convenience function similar to above that adds multiple directories.
   */
  void addLibraryDirs(const QStringList& dirs);

  /**
   * Adds a plugin that will be loaded and used by the factory. This is automatically added to
   * the list of files/libraries, so no need to add it with addLibraryDir, or addFile.
   */
  void addPlugin(const std::string& lib) { _plugins.push_back(lib); }

  Configuration& getConfiguration() { return _conf; }

  static const std::string& getDefaultJobTracker() { return _defaultTracker; }

  std::string getJobTracker() const { return _conf.get("mapred.job.tracker"); }

  const std::vector<std::string>& getLibraryDirs() const { return _libraryDir; }

  void run();

  /**
   * Sets the job tracker for this job. Pipes does not support the "local" job tracker. To work
   * around this PrettyPipes implements its own internal version of "local" it runs very quickly,
   * but not all Hadoop functions are supported. It maintains all intermediate data in RAM
   * so expect bad things to happen with large jobs. While multiple reducers are supported they
   * are executed sequentially in a single thread.
   *
   * @sa setDefaultJobTracker
   */
  void setJobTracker(const std::string& tracker) { _conf.set("mapred.job.tracker", tracker); }

  void setInput(const std::string& input) { _input = input; _conf.set("mapred.input.dir", input); }

  void setInputFormatClass(const std::string& inputFormat);

  void setMapperClass(const std::string& mapper);

  void setName(const std::string& name) { _conf.set("mapred.job.name", name); }

  void setNumReduceTask(int n) { _conf.setInt("mapred.reduce.tasks", n); }

  void setOutput(const std::string& output);

  void setRecordReaderClass(const std::string& reader);

  void setRecordWriterClass(const std::string& writer);

  void setReducerClass(const std::string& reducer);

  void setVerbose(bool verbose) { _verbose = verbose; }

  /**
   * Sets that job tracker that all new Jobs will use. This is most useful when running unit tests.
   * With unit tests you can set the job tracker to "local" once and all unit tests will run
   * locally.
   *
   * @sa setJobTracker
   */
  static void setDefaultJobTracker(const std::string& tracker) { _defaultTracker = tracker; }

private:
  Configuration _conf;
  static std::string _defaultTracker;
  std::string _input;
  std::string _output;
  std::vector<std::string> _libraryDir;
  std::vector<std::string> _plugins;
  std::vector<std::string> _files;
  QStringList _opts;
  bool _verbose;

  QString _getFiles();

  QString _getPpHome();

  void _runHadoop();

  void _runLocal();

  void _uploadBinary();

  void _writeConfiguration(QFile& f);
};

}

#endif // JOB_H
