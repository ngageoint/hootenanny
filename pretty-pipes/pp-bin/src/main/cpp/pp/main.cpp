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
 * @copyright Copyright (C) 2015, 2016, 2017 Maxar (http://www.maxar.com/)
 */

// The Hadoop "emit" function overlaps with Qt's emit keyword.
#define QT_NO_EMIT

// Boost
#include <boost/any.hpp>
using namespace boost;

// GCC
#include <execinfo.h>

// PrettyPipes
#include <pp/Factory.h>
#include <pp/Exception.h>
#include <pp/MapContextConsumer.h>
#include <pp/ReduceContextConsumer.h>
#include <pp/Strings.h>
#include <pp/mapreduce/NullMapper.h>
#include <pp/mapreduce/NullReducer.h>
#include <pp/RecordReader.h>
#include <pp/mapreduce/RecordWriter.h>

// Qt
#include <QCoreApplication>
#include <QDir>
#include <QLibrary>
#include <QString>
#include <QStringList>

// Standard
#include <arpa/inet.h>
#include <cxxabi.h>
#include <endian.h>
#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>

using namespace std;

class MyFactory : public HadoopPipes::Factory
{
public:

  mutable bool _init;

  MyFactory() : _init(false)
  {
  }

  void _initialize(const HadoopPipes::JobConf* conf) const
  {
    if (_init == false)
    {
      QString ldLib(getenv("LD_LIBRARY_PATH"));
      cout << "LD_LIBRARY_PATH start: " << ldLib.toStdString() << endl;
      const char* javaHome = getenv("JAVA_HOME");
      if (javaHome == NULL)
      {
        throw pp::Exception("JAVA_HOME is not set.");
      }
      const char* hadoopHome = getenv("HADOOP_HOME");
      if (hadoopHome == NULL)
      {
        throw pp::Exception("HADOOP_HOME is not set.");
      }
      if (ldLib.endsWith(":") == false && ldLib.isEmpty() == false)
      {
        ldLib.append(":");
      }
      ldLib += QString("%1/jre/lib/amd64:%2/jre/lib/amd64/server:%3/c++/Linux-amd64-64/lib/").
          arg(javaHome).arg(javaHome).arg(hadoopHome);
      setenv("LD_LIBRARY_PATH", ldLib.toUtf8().constData(), 1);
      cout << "LD_LIBRARY_PATH: " << ldLib.toStdString() << endl;

      string plugins = conf->get(PP_PLUGINS);
      cout << "plugins: " << plugins << endl;
      QStringList l = QString::fromStdString(plugins).split(",");
      for (int i = 0; i < l.size(); i++)
      {
        cout << "Loading " << l[i].toStdString() << endl;
        QLibrary lib(l[i]);
        if (lib.load() == false)
        {
          QString message = "Error loading library: " + l[i] + "\n";
          message += "LD_LIBRARY_PATH: " + ldLib + "\n";
          message += "JAVA_HOME: " + QString(javaHome) + "\n";
          message += "HADOOP_HOME: " + QString(hadoopHome) + "\n";
          throw pp::Exception(message);
        }
      }
      _init = true;
    }

  }

  template<class T>
  T* createObject(const HadoopPipes::JobConf* conf, const string& key) const
  {
    _initialize(conf);
    if (conf->hasKey(key) == false)
    {
      throw pp::Exception("Error finding specified key.");
    }
    return pp::Factory::getInstance().constructObject<T>(conf->get(key));
  }

  virtual HadoopPipes::Mapper* createMapper(HadoopPipes::MapContext& context) const
  {
    if (context.getJobConf()->hasKey(PP_MAPPER) &&
        context.getJobConf()->get(PP_MAPPER) != "")
    {
      return createObject<pp::Mapper>(context.getJobConf(), PP_MAPPER);
    }
    else
    {
      return new pp::NullMapper();
    }
  }

  virtual HadoopPipes::Reducer* createReducer(HadoopPipes::ReduceContext& context) const
  {
    if (context.getJobConf()->hasKey(PP_REDUCER) &&
        context.getJobConf()->get(PP_REDUCER) != "")
    {
      return createObject<pp::Reducer>(context.getJobConf(), PP_REDUCER);
    }
    else
    {
      return new pp::NullReducer();
    }
  }

  /**
   * Create a combiner, if this application has one.
   * @return the new combiner or NULL, if one is not needed
   */
  virtual HadoopPipes::Reducer* createCombiner(HadoopPipes::MapContext& context) const {
    if (context.getJobConf()->hasKey(PP_COMBINER))
    {
      return createObject<pp::Reducer>(context.getJobConf(), PP_COMBINER);
    }
    else
    {
      return NULL;
    }
  }

  /**
   * Create an application partitioner object.
   * @return the new partitioner or NULL, if the default partitioner should be
   *     used.
   */
  virtual HadoopPipes::Partitioner* createPartitioner(HadoopPipes::MapContext&) const {
    return NULL;
  }

  /**
   * Create an application record reader.
   * @return the new RecordReader or NULL, if the Java RecordReader should be
   *    used.
   */
  virtual HadoopPipes::RecordReader* createRecordReader(HadoopPipes::MapContext& context) const {
    pp::RecordReader* result = NULL;
    if (context.getJobConf()->hasKey(PP_RECORD_READER))
    {
      result = createObject<pp::RecordReader>(context.getJobConf(), PP_RECORD_READER);
      pp::MapContextConsumer* consumer = dynamic_cast<pp::MapContextConsumer*>(result);
      if (consumer != NULL)
      {
        cout << "Setting map context." << endl;
        consumer->setMapContext(context);
      }
      else
      {
        cout << "Not setting map context." << endl;
      }
      cout.flush();
    }
    return result;
  }

  /**
   * Create an application record writer.
   * @return the new RecordWriter or NULL, if the Java RecordWriter should be
   *    used.
   */
  virtual HadoopPipes::RecordWriter* createRecordWriter(HadoopPipes::ReduceContext& context) const
  {
    pp::RecordWriter* result = NULL;
    if (context.getJobConf()->hasKey(PP_RECORD_WRITER))
    {
      result = createObject<pp::RecordWriter>(context.getJobConf(), PP_RECORD_WRITER);
      pp::ReduceContextConsumer* consumer = dynamic_cast<pp::ReduceContextConsumer*>(result);
      if (consumer != NULL)
      {
        cout << "Setting record writer reduce context." << endl;
        consumer->setReduceContext(context);
      }
      else
      {
        cout << "Not setting record writer reduce context." << endl;
      }
      cout.flush();
    }
    return result;
  }

};

/** Print a demangled stack backtrace of the caller function to FILE* out. */
static inline void print_stacktrace(FILE *out = stderr, unsigned int max_frames = 63)
{
    fprintf(out, "stack trace:\n");

    // storage array for stack trace address data
    void* addrlist[max_frames+1];

    // retrieve current stack addresses
    int addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void*));

    if (addrlen == 0) {
        fprintf(out, "  <empty, possibly corrupt>\n");
        return;
    }

    // resolve addresses into strings containing "filename(function+address)",
    // this array must be free()-ed
    char** symbollist = backtrace_symbols(addrlist, addrlen);

    // allocate string which will be filled with the demangled function name
    size_t funcnamesize = 256;
    char* funcname = (char*)malloc(funcnamesize);

    // iterate over the returned symbol lines. skip the first, it is the
    // address of this function.
    for (int i = 1; i < addrlen; i++)
    {
        char *begin_name = 0, *begin_offset = 0, *end_offset = 0;

        // find parentheses and +address offset surrounding the mangled name:
        // ./module(function+0x15c) [0x8048a6d]
        for (char *p = symbollist[i]; *p; ++p)
        {
            if (*p == '(')
                begin_name = p;
            else if (*p == '+')
                begin_offset = p;
            else if (*p == ')' && begin_offset) {
                end_offset = p;
                break;
            }
        }

        if (begin_name && begin_offset && end_offset
            && begin_name < begin_offset)
        {
            *begin_name++ = '\0';
            *begin_offset++ = '\0';
            *end_offset = '\0';

            // mangled name is now in [begin_name, begin_offset) and caller
            // offset in [begin_offset, end_offset). now apply
            // __cxa_demangle():

            int status;
            char* ret = abi::__cxa_demangle(begin_name,
                                            funcname, &funcnamesize, &status);
            if (status == 0) {
                funcname = ret; // use possibly realloc()-ed string
                //fprintf(out, "  %s : %s+%s\n",
                //        symbollist[i], funcname, begin_offset);
                fprintf(out, "  %s +%s\n", funcname, begin_offset);
            }
            else {
                // demangling failed. Output function name as a C function with
                // no arguments.
                fprintf(out, "  %s : %s()+%s\n",
                        symbollist[i], begin_name, begin_offset);
            }
        }
        else
        {
            // couldn't parse the line? print the whole line.
            fprintf(out, "  %s\n", symbollist[i]);
        }
    }

    free(funcname);
    free(symbollist);
}


void handler(int sig)
{
  // print out all the frames to stderr
  fprintf(stderr, "Error: signal %d:\n", sig);

  print_stacktrace();

  fflush(stderr);
  exit(-1);
}

void terminateHandler()
{
  static bool tried_throw = false;

  try
  {
    // try once to re-throw currently active exception
    if (!tried_throw)
    {
      tried_throw = true;
      throw;
    }
  }
  catch (const std::exception &e)
  {
    std::cerr << __FUNCTION__ << " caught unhandled exception. what(): "
              << e.what() << std::endl;
  }
  catch (...)
  {
    std::cerr << __FUNCTION__ << " caught unknown/unhandled exception."
              << std::endl;
  }

  print_stacktrace();
  fflush(stderr);
  abort();
}

int main(int argc, char *argv[])
{
  cout << "Starting PpBin" << endl << flush;

  setenv("LIBHDFS_OPTS", "-Xmx256M", 1);
  struct rlimit rl;
  getrlimit(RLIMIT_AS, &rl);
  // Set limit to 6GB
  rl.rlim_cur = 6l * 1024l * 1024l * 1024l;
  setrlimit(RLIMIT_AS, &rl);
  cout << "Set RLIMIT_AS to " << rl.rlim_cur << endl << flush;

  QString hadoopHome = getenv("HADOOP_HOME");
  QString javaHome = getenv("JAVA_HOME");

  QStringList classPath;
  QStringList jars;
  jars << "*.jar" << "*.JAR";
  QFileInfoList fiLib = QDir(hadoopHome + "/lib/").entryInfoList(jars, QDir::Files, QDir::Name);
  for (int i = 0; i < fiLib.size(); i++)
  {
    classPath << fiLib[i].absoluteFilePath();
  }
  QFileInfoList fi = QDir(hadoopHome).entryInfoList(jars, QDir::Files, QDir::Name);
  for (int i = 0; i < fi.size(); i++)
  {
    classPath << fi[i].absoluteFilePath();
  }
  classPath << javaHome + "/lib/tools.jar";
  classPath << hadoopHome + "/conf/";

  setenv("CLASSPATH", classPath.join(":").toUtf8().data(), 1);
  //setenv("LIBHDFS_OPTS", ("-classpath " + classPath).toUtf8().data(), 1);
  cout << "CLASSPATH: " << getenv("CLASSPATH") << endl << flush;
  //cout << "LIBHDFS_OPTS: " << getenv("LIBHDFS_OPTS") << endl << flush;

  // These _might_ work depending on the specific error.
  std::set_terminate(terminateHandler);
  signal(SIGSEGV, handler);
  signal(SIGABRT, handler);

  QCoreApplication app(argc, argv);

  cout << "Running task." << endl;
  cout.flush();
  return HadoopPipes::runTask(MyFactory());
}
