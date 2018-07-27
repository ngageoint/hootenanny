
#include "FormatsDisplayer.h"

// Hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/io/OgrUtilities.h>
#include <hoot/core/io/OsmMapReader.h>
#include <hoot/core/io/OsmMapWriter.h>

namespace hoot
{

template<typename IoClass>
void printFormats(const std::string& className, const QStringList extraFormats = QStringList())
{
  std::vector<std::string> readerNames =
    Factory::getInstance().getObjectNamesByBase(className);
  QSet<QString> formats;
  for (size_t i = 0; i < readerNames.size(); i++)
  {
    boost::shared_ptr<IoClass> c(
      Factory::getInstance().constructObject<IoClass>(readerNames[i]));
    const QString supportedFormats = c->supportedFormats();
    if (!supportedFormats.isEmpty())
    {
      QStringList supportedFormatsList = supportedFormats.split(";");
      for (int j = 0; j < supportedFormatsList.size(); j++)
      {
        formats.insert(supportedFormatsList.at(j));
      }
    }
  }
  formats += OgrUtilities::getInstance().getSupportedFormats(true);
  QStringList formatsList = formats.toList();
  formatsList.append(extraFormats);
  formatsList.sort();

  for (int i = 0; i < formatsList.size(); i++)
  {
    std::cout << formatsList.at(i) << std::endl;
  }
  std::cout << std::endl;
}

void FormatsDisplayer::display(const bool displayInputs, const bool displayOutputs)
{
  DisableLog dl;

  if (displayInputs)
  {
    std::cout << "Input formats:" << std::endl << std::endl;
    printFormats<OsmMapReader>(OsmMapReader::className());
  }

  if (displayOutputs)
  {
    std::cout << "Output formats:" << std::endl << std::endl;
    QStringList formatsList;
    //These are supported by the changeset writers, who aren't map readers/writers.  Possibly,
    //a lightweight interface could be used on all the readers writers instead of modifying
    //OsmMapReader/OsmMapwriter with the supportedFormats method to make this better.
    formatsList.append(".osc");
    formatsList.append(".osc.sql");
    printFormats<OsmMapWriter>(OsmMapWriter::className(), formatsList);
  }
}

}
