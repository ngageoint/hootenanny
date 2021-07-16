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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/criterion/MultiUseBuildingCriterion.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/criterion/OrCriterion.h>
#include <hoot/core/criterion/ParallelWayCriterion.h>
#include <hoot/core/criterion/WayBufferCriterion.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

class ElementCriterionCloneTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ElementCriterionCloneTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBasicTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    std::vector<QString> classNames =
      Factory::getInstance().getObjectNamesByBase(ElementCriterion::className());
    std::sort(classNames.begin(), classNames.end());
    for (size_t i = 0; i < classNames.size(); i++)
    {
      const QString className = classNames[i];
      LOG_VART(className);
      // TODO: These crash the test. Need to figure out what to do about then.
      if (className == NotCriterion::className() || className == OrCriterion::className() ||
          className == ParallelWayCriterion::className() ||
          className == WayBufferCriterion::className() ||
          className == MultiUseBuildingCriterion::className())
      {
        continue;
      }
      ElementCriterionPtr crit =
        Factory::getInstance().constructObject<ElementCriterion>(className);
      LOG_VART(crit.get());

      std::shared_ptr<ConstOsmMapConsumer> mapConsumer =
        std::dynamic_pointer_cast<ConstOsmMapConsumer>(crit);
      if (mapConsumer)
      {
        mapConsumer->setOsmMap(map.get());
      }

      QString str = crit->getName();
      LOG_VART(str);
      str = crit->getClassName();
      LOG_VART(str);
      str = crit->toString();
      LOG_VART(str);
      ElementCriterionPtr clone = crit->clone();
      CPPUNIT_ASSERT(clone != nullptr);
    }
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ElementCriterionCloneTest, "quick");

}
