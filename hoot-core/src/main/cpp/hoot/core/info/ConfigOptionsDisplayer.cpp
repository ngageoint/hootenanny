
#include "ConfigOptionsDisplayer.h"

// Hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/HootException.h>

// Qt
#include <QFile>

namespace hoot
{

void ConfigOptionsDisplayer::displayAllOptionNames(const bool withDetails)
{
  const QString configOptionsFile = ConfigOptions().getConfigOptionsFile();
  QString cmd;
  LOG_VARD(withDetails);
  if (!withDetails)
  {
    cmd = "cat " + configOptionsFile + " | grep '^===' | sed 's/=== //g'";
  }
  else
  {
    cmd = "cat " + configOptionsFile + " | grep -v '//'";
  }

  if (!cmd.isEmpty() && std::system(cmd.toStdString().c_str()) != 0)
  {
    throw HootException("Unable to list configuration options.");
  }
}

void ConfigOptionsDisplayer::displayOptionName(const QString optionName, const bool withDetails)
{
  const QString configOptionsFile = ConfigOptions().getConfigOptionsFile();
  QString cmd;
  LOG_VARD(optionName);
  LOG_VARD(withDetails);
  if (!withDetails)
  {
    cmd =
      "cat " + configOptionsFile + " | grep '^=== " + optionName.toLower().trimmed() +
      "' | sed 's/=== //g'";
  }
  else
  {
    _printAllConfigOptionsDetails(optionName, configOptionsFile);
  }

  if (!cmd.isEmpty() && std::system(cmd.toStdString().c_str()) != 0)
  {
    throw HootException("Unable to list configuration options.");
  }
}

void ConfigOptionsDisplayer::_printAllConfigOptionsDetails(const QString optionName,
                                                           const QString configOptionsFile)
{
  QFile file(configOptionsFile);
  if (!file.open(QFile::ReadOnly))
  {
    throw HootException("Error opening file for reading: " + configOptionsFile);
  }

  //TODO: replace this w/ some fancy lookahead regex called by grep...doing it by brute force for
  //now
  QString output;
  try
  {
    bool foundOption = false;
    while (!file.atEnd())
    {
      const QString line = QString::fromUtf8(file.readLine().constData());

      if (line.startsWith("=== " + optionName))
      {
        foundOption = true;
      }
      else if (line.startsWith("=== "))
      {
        foundOption = false;
      }

      if (foundOption)
      {
        output += line ;
      }
    }
  }
  catch (...)
  {
    file.close();
  }
  file.close();

  std::cout << output.trimmed() << std::endl;
}

}
