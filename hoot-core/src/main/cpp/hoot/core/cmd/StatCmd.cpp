
// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/visitors/SingleStatistic.h>
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/util/Exception.h>
#include <hoot/core/io/OsmMapReader.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

class StatCmd : public BaseCommand
{
public:

  static std::string className() { return "hoot::StatCmd"; }

  CountFeaturesCmd() { }

  virtual QString getName() const { return "stat"; }

  virtual QString getDescription() const
  { return "Calculates a statistic on features"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() != 2)
    {
      LOG_VAR(args);
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes two parameters.").arg(getName()));
    }

    const QStringList inputs = args[0].split(";");
    LOG_VART(inputs.size());
    OsmMapPtr map(new OsmMap());
    _readInputs(inputs, map);

    LOG_VARD(args[1]);
    _applyOperator(args[1], map);

    return 0;
  }

private:

  void _readInputs(const QStringList inputs, OsmMapPtr map)
  {
    for (int i = 0; i < inputs.size(); i++)
    {
      const QString input = inputs.at(i);
      boost::shared_ptr<OsmMapReader> reader =
        OsmMapReaderFactory::getInstance().createReader(input, true, Status::Unknown1);
      reader->open(input);
      reader->read(map);
    }
  }

  void _applyOperator(const QString visClassName, OsmMapPtr map)
  {
    double total = 0;

    //filter the elements

    boost::shared_ptr<ConstElementVisitor> statsCollector;
    boost::shared_ptr<SingleStatistic> singleStat;
    try
    {
      statsCollector.reset(
        Factory::getInstance().constructObject<ConstElementVisitor>(visClassName));
      singleStat.reset(boost::dynamic_pointer_cast<SingleStatistic>(statsCollector));
    }
    catch (const boost::bad_any_cast&)
    {
      throw IllegalArgumentException("Invalid visitor: " + visClassName);
    }
    LOG_VART(statsCollector.get());
    LOG_VART(singleStat.get());

    boost::shared_ptr<Configurable> visConfig =
      boost::dynamic_pointer_cast<Configurable>(statsCollector);
    LOG_VART(visConfig.get());
    if (visConfig.get())
    {
      visConfig->setConfiguration(conf());
    }

    LOG_TRACE("Using visitor with SingleStatistic...");
    map->visitRo(*statsCollector);
    total = singleStat->getStat();
    LOG_VART(total);

    std::cout << "Total: " << total << std::endl;
  }
};

HOOT_FACTORY_REGISTER(Command, StatCmd)

}

