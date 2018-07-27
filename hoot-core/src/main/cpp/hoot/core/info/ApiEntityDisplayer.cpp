
#include "ApiEntityDisplayer.h"

// Hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/visitors/SingleStatistic.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/conflate/extractors/FeatureExtractor.h>
#include <hoot/core/conflate/matching/MatchCreator.h>
#include <hoot/core/conflate/merging/MergerCreator.h>
#include <hoot/core/schema/TagMerger.h>

namespace hoot
{

template<typename ApiEntity>
class ApiEntityNameComparator
{
public:

  ApiEntityNameComparator() {}

  bool operator()(const std::string& name1, const std::string& name2)
  {
    return
      QString::fromStdString(name1).replace("hoot::", "") <
      QString::fromStdString(name2).replace("hoot::", "");
  }
};

template<typename ApiEntity>
void printApiEntities(const std::string& apiEntityClassName, const QString apiEntityType,
                      const bool displayType)
{
  //the size of the longest names plus a 3 space buffer
  const int maxNameSize = 45;
  const int maxTypeSize = 18;

  std::vector<std::string> cmds =
    Factory::getInstance().getObjectNamesByBase(apiEntityClassName);
  ApiEntityNameComparator<ApiEntity> apiEntityNameComparator;
  std::sort(cmds.begin(), cmds.end(), apiEntityNameComparator);
  for (size_t i = 0; i < cmds.size(); i++)
  {
    LOG_VARD(cmds[i]);
    boost::shared_ptr<ApiEntity> c(
      Factory::getInstance().constructObject<ApiEntity>(cmds[i]));
    if (!c->getDescription().isEmpty())
    {
      bool supportsSingleStat = false;
      boost::shared_ptr<SingleStatistic> singleStat =
        boost::dynamic_pointer_cast<SingleStatistic>(c);
      if (singleStat.get())
      {
        supportsSingleStat = true;
      }

      QString name = QString::fromStdString(cmds[i]).replace("hoot::", "");
      //append '*' to the names of visitors that support the SingleStatistic interface
      if (supportsSingleStat)
      {
        name += "*";
      }
      const int indentAfterName = maxNameSize - name.size();
      const int indentAfterType = maxTypeSize - apiEntityType.size();
      QString line = "  " + name + QString(indentAfterName, ' ');
      if (displayType)
      {
        line += apiEntityType + QString(indentAfterType, ' ');
      }
      line += c->getDescription();
      std::cout << line << std::endl;
    }
  }
  std::cout << std::endl;
}

//matchers/mergers have a more roundabout way to get at the description, so we'll create a new
//display method for them
template<typename ApiEntity>
void printApiEntities2(const std::string& apiEntityClassName)
{
  //the size of the longest names plus a 3 space buffer
  const int maxNameSize = 48;

  std::vector<std::string> names =
    Factory::getInstance().getObjectNamesByBase(apiEntityClassName);
  ApiEntityNameComparator<ApiEntity> apiEntityNameComparator;
  std::sort(names.begin(), names.end(), apiEntityNameComparator);
  LOG_VARD(names);
  QStringList output;
  for (size_t i = 0; i < names.size(); i++)
  {
    // get all names known by this creator
    boost::shared_ptr<ApiEntity> mc(
      Factory::getInstance().constructObject<ApiEntity>(names[i]));
    std::vector<CreatorDescription> creators = mc->getAllCreators();
    LOG_VARD(creators.size());

    for (std::vector<CreatorDescription>::const_iterator itr = creators.begin();
         itr != creators.end(); ++itr)
    {
      CreatorDescription description = *itr;
      LOG_VARD(description);
      const QString name = QString::fromStdString(description.className).replace("hoot::", "");
      LOG_VARD(name);
      //this suppresses test and auxiliary rules files
      if (!name.endsWith("Test.js") && !name.endsWith("Rules.js"))
      {
        const int indentAfterName = maxNameSize - name.size();
        QString line = "  " + name + QString(indentAfterName, ' ');
        if (description.experimental)
        {
          line += "(experimental) ";
        }
        line += description.description;
        LOG_VARD(line);
        output.append(line);
      }
    }
  }

  output.sort();
  for (int i = 0; i < output.size(); i++)
  {
    std::cout << output.at(i) << std::endl;
  }
}

void ApiEntityDisplayer::display(const QString apiEntityType)
{
  DisableLog dl;
  QString msg = " (prepend 'hoot::' before using; * = implements SingleStatistic):";
  if (apiEntityType == "operators")
  {
    msg.prepend("Operators");
    std::cout << msg << std::endl << std::endl;

    printApiEntities<ElementCriterion>(ElementCriterion::className(), "criterion", true);
    printApiEntities<OsmMapOperation>(OsmMapOperation::className(), "operation", true);
    //TODO: would like to combine these into one, as far as the display is concerned, somehow
    printApiEntities<ElementVisitor>(ElementVisitor::className(), "visitor", true);
    printApiEntities<ConstElementVisitor>(
      ConstElementVisitor::className(), "visitor (const)", true);
  }
  else if (apiEntityType == "feature-extractors")
  {
    msg.prepend("Feature Extractors");
    std::cout << msg << std::endl << std::endl;
    printApiEntities<FeatureExtractor>(FeatureExtractor::className(), "feature extractor", false);
  }
  else if (apiEntityType == "matchers")
  {
    msg.prepend("Conflate Matchers");
    std::cout << msg << std::endl << std::endl;
    printApiEntities2<MatchCreator>(MatchCreator::className());
  }
  else if (apiEntityType == "mergers")
  {
    msg.prepend("Conflate Mergers");
    std::cout << msg << std::endl << std::endl;
    printApiEntities2<MergerCreator>(MergerCreator::className());
  }
  else if (apiEntityType == "tag-mergers")
  {
    msg.prepend("Tag Mergers");
    std::cout << msg << std::endl << std::endl;
    printApiEntities<TagMerger>(TagMerger::className(), "tag merger", false);
  }
}

}
