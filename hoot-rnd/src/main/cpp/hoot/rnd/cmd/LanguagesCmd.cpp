
// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/util/Log.h>
#include <hoot/rnd/language/translators/HootServicesTranslationInfoClient.h>

// Boost
#include <boost/foreach.hpp>

// Qt
#include <QUrl>

namespace hoot
{

class LanguagesCmd : public BaseCommand
{
public:

  static std::string className() { return "hoot::LanguagesCmd"; }

  LanguagesCmd() { }

  virtual QString getName() const { return "languages"; }

  virtual QString getDescription() const
  { return "Displays information about translatable/detectable spoken languages"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() != 1)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes one parameter.").arg(getName()));
    }

    //only allowing one option per command
    const QStringList supportedOpts = _getSupportedOptions();
    QStringList specifiedOpts;
    for (int i = 0; i < args.size(); i++)
    {
      const QString arg = args.at(i);
      if (specifiedOpts.contains(arg) || (supportedOpts.contains(arg) && specifiedOpts.size() > 0))
      {
        std::cout << getHelp() << std::endl << std::endl;
        throw HootException(QString("%1 takes a single option.").arg(getName()));
      }
      specifiedOpts.append(arg);
    }
    if (specifiedOpts.size() == 0)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes a single option.").arg(getName()));
    }
    LOG_VARD(specifiedOpts.size());

    const QString type = args[0].replace("--", "");
    HootServicesTranslationInfoClient client;
    client.setConfiguration(conf());
    boost::property_tree::ptree replyObj = client.getAvailableLanguages(type);

    std::cout << type << " languages: " << std::endl << std::endl;
    int langCtr = 0;
    int availableCtr = 0;
    BOOST_FOREACH (boost::property_tree::ptree::value_type& language, replyObj.get_child("languages"))
    {
      const QString langName =
        QUrl::fromPercentEncoding(
          QString::fromStdString(language.second.get<std::string>("name")).toUtf8());
      const bool available = language.second.get<bool>("available");
      const QString availableStr = available ? "yes" : "no";

      std::cout << "Language code: " << QString::fromStdString(language.second.get<std::string>("iso6391code")) << std::endl;
      std::cout << "Language name: " << langName << std::endl;
      std::cout << "Language available: " << availableStr << std::endl;
      std::cout << std::endl;
      langCtr++;
      if (available)
      {
        availableCtr++;
      }
    }

    std::cout << QString::number(langCtr) << " languages are supported for " << type << ".  " << std::endl;
    std::cout << QString::number(availableCtr) << " of those languages are currently available for " << type << ".  " << std::endl;

    return 0;
  }

private:

  QStringList _getSupportedOptions() const
  {
    QStringList options;
    options.append("--detectable");
    options.append("--translatable");
    return options;
  }
};

HOOT_FACTORY_REGISTER(Command, LanguagesCmd)

}

