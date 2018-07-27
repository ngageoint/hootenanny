
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

//static bool extractorCompare(const std::string& n1, const std::string& n2)
//{
//  const QString cleanedName1 = QString::fromStdString(n1).replace("hoot::", "");
//  const QString cleanedName2 = QString::fromStdString(n2).replace("hoot::", "");
//  return cleanedName1 < cleanedName2;
//}

template<typename ApiEntity>
void printApiEntities(const std::string& apiEntityClassName, const QString apiEntityType)
{
  //the size of the longest operator/type names plus a 3 space buffer
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
      const QString line =
        "  " + name + QString(indentAfterName, ' ') + apiEntityType +
        QString(indentAfterType, ' ') + c->getDescription();
      std::cout << line << std::endl;
    }
  }
  std::cout << std::endl;
}

void ApiEntityDisplayer::display(const QString apiEntityType)
{
  /*
   * feature-extractors
   * //the size of the longest extractor name plus a small buffer
    const int indent = 40;
    std::vector<std::string> extractorNames =
      Factory::getInstance().getObjectNamesByBase(FeatureExtractor::className());
    std::sort(extractorNames.begin(), extractorNames.end(), extractorCompare);
    for (size_t i = 0; i < extractorNames.size(); i++)
    {
      boost::shared_ptr<FeatureExtractor> c(
        Factory::getInstance().constructObject<FeatureExtractor>(extractorNames[i]));
      const QString cleanedName = QString::fromStdString(extractorNames[i]).replace("hoot::", "");
      if (!cleanedName.isEmpty() && !c->getDescription().isEmpty())
      {
        LOG_VARD(cleanedName);
        const int spaceSize = indent - cleanedName.size();
        const QString line = "  " + cleanedName + QString(spaceSize, ' ') + c->getDescription();
        std::cout << line << std::endl;
      }
    }
   */

  DisableLog dl;
  if (apiEntityType == "operators")
  {
    std::cout << "Operators (prepend 'hoot::'):" << std::endl << std::endl;

    printApiEntities<ElementCriterion>(ElementCriterion::className(), "criterion");
    printApiEntities<OsmMapOperation>(OsmMapOperation::className(), "operation");
    //TODO: would like to combine these into one, as far as the display is concerned, somehow
    printApiEntities<ElementVisitor>(ElementVisitor::className(), "visitor");
    printApiEntities<ConstElementVisitor>(ConstElementVisitor::className(), "visitor (const)");
  }
  else if (apiEntityType == "feature-extractors")
  {
    std::cout << "Feature Extractors (prepend 'hoot::'):" << std::endl << std::endl;
    printApiEntities<FeatureExtractor>(FeatureExtractor::className(), "feature extractor");
  }
  else if (apiEntityType == "matchers")
  {
    std::cout << "Conflate Matchers (prepend 'hoot::'):" << std::endl << std::endl;
    printApiEntities<MatchCreator>(MatchCreator::className(), "matcher");
  }
  else if (apiEntityType == "mergers")
  {
    std::cout << "Conflate Mergers (prepend 'hoot::'):" << std::endl << std::endl;
    printApiEntities<MergerCreator>(MergerCreator::className(), "merger");
  }
  else if (apiEntityType == "tag-mergers")
  {
    std::cout << "Tag Mergers (prepend 'hoot::'):" << std::endl << std::endl;
    printApiEntities<TagMerger>(TagMerger::className(), "tag merger");
  }
}

}
