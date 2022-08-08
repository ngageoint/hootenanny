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
 * @copyright Copyright (C) 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/criterion/OrCriterion.h>
#include <hoot/core/criterion/ParallelWayCriterion.h>
#include <hoot/core/criterion/WayBufferCriterion.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/criterion/FilteredByGeometryTypeCriteria.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/visitors/ElementVisitor.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

/**
 * In this test, we simply check that the FilteredByGeometryTypeCriteria calls don't crash. This
 * test is kind of a cheap way to increase test coverage, so we may be able to come up with
 * something better.
 */
class FilteredByGeometryTypeCriteriaTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(FilteredByGeometryTypeCriteriaTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBasicTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();

    std::vector<QString> critClassNames =
      Factory::getInstance().getObjectNamesByBase(ElementCriterion::className());
    std::sort(critClassNames.begin(), critClassNames.end());
    for (size_t i = 0; i < critClassNames.size(); i++)
    {
      const QString className = critClassNames[i];
      LOG_VART(className);
      // See related note in ElementCriterionCloneTest.
      if (className == NotCriterion::className() || className == OrCriterion::className() ||
          className == ParallelWayCriterion::className() ||
          className == WayBufferCriterion::className())
      {
        continue;
      }

      ElementCriterionPtr crit =
        Factory::getInstance().constructObject<ElementCriterion>(className);
      LOG_VART(crit.get());
      QString str = crit->getName();
      LOG_VART(str);
      str = crit->getClassName();
      LOG_VART(str);
      str = crit->toString();
      LOG_VART(str);

      std::shared_ptr<OsmMapConsumer> mapConsumer =
        std::dynamic_pointer_cast<OsmMapConsumer>(crit);
      if (mapConsumer)
      {
        mapConsumer->setOsmMap(map.get());
      }

      std::shared_ptr<Configurable> config =
        std::dynamic_pointer_cast<Configurable>(crit);
      if (config)
      {
        // Need to catch here, b/c some api entities have invalid default configurations, which is
        // done on purpose. Doing so forces the caller to populate the correct configuration values.
        // In this test, we're only checking that the OperationStatus calls return sane values, so
        // a failing configuration *shouldn't* really matter. If it does end up mattering for some
        // api entity, then it will either have to be populated with a valid default configuration
        // or excluded from this test.
        try
        {
          config->setConfiguration(conf());
        }
        catch (...)
        {
        }
      }

      std::shared_ptr<FilteredByGeometryTypeCriteria> filteredByCrit =
        std::dynamic_pointer_cast<FilteredByGeometryTypeCriteria>(crit);
      if (filteredByCrit)
      {
        // We can only get the crit list here and not require it to be non-empty. This is b/c
        // FilteredByGeometryTypeCriteria is given an empty implementation at the top level of api
        // entities, and not all entities actually provide a meaningful implementation for it.
        const QStringList crits = filteredByCrit->getCriteria();
        LOG_VART(crits);
      }
    }

    std::vector<QString> opClassNames =
      Factory::getInstance().getObjectNamesByBase(OsmMapOperation::className());
    std::sort(opClassNames.begin(), opClassNames.end());
    for (size_t i = 0; i < opClassNames.size(); i++)
    {
      const QString className = opClassNames[i];
      LOG_VART(className);

      std::shared_ptr<OsmMapOperation> op =
        Factory::getInstance().constructObject<OsmMapOperation>(className);
      LOG_VART(op.get());
      QString str = op->getName();
      LOG_VART(str);
      str = op->getClassName();
      LOG_VART(str);
      str = op->toString();
      LOG_VART(str);

      std::shared_ptr<OsmMapConsumer> mapConsumer =
        std::dynamic_pointer_cast<OsmMapConsumer>(op);
      if (mapConsumer)
      {
        mapConsumer->setOsmMap(map.get());
      }

      std::shared_ptr<Configurable> config =
        std::dynamic_pointer_cast<Configurable>(op);
      if (config)
      {
        try
        {
          config->setConfiguration(conf());
        }
        catch (...)
        {
        }
      }

      std::shared_ptr<FilteredByGeometryTypeCriteria> filteredByCrit =
        std::dynamic_pointer_cast<FilteredByGeometryTypeCriteria>(op);
      if (filteredByCrit)
      {
        const QStringList crits = filteredByCrit->getCriteria();
        LOG_VART(crits);
      }
    }

    std::vector<QString> visClassNames =
      Factory::getInstance().getObjectNamesByBase(ElementVisitor::className());
    std::sort(visClassNames.begin(), visClassNames.end());
    for (size_t i = 0; i < visClassNames.size(); i++)
    {
      const QString className = visClassNames[i];
      LOG_VART(className);

      std::shared_ptr<ElementVisitor> vis =
        Factory::getInstance().constructObject<ElementVisitor>(className);
      LOG_VART(vis.get());
      QString str = vis->getName();
      LOG_VART(str);
      str = vis->getClassName();
      LOG_VART(str);
      str = vis->toString();
      LOG_VART(str);

      std::shared_ptr<OsmMapConsumer> mapConsumer =
        std::dynamic_pointer_cast<OsmMapConsumer>(vis);
      if (mapConsumer)
      {
        mapConsumer->setOsmMap(map.get());
      }

      std::shared_ptr<Configurable> config =
        std::dynamic_pointer_cast<Configurable>(vis);
      if (config)
      {
        try
        {
          config->setConfiguration(conf());
        }
        catch (...)
        {
        }
      }

      std::shared_ptr<FilteredByGeometryTypeCriteria> filteredByCrit =
        std::dynamic_pointer_cast<FilteredByGeometryTypeCriteria>(vis);
      if (filteredByCrit)
      {
        const QStringList crits = filteredByCrit->getCriteria();
        LOG_VART(crits);
      }
    }
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(FilteredByGeometryTypeCriteriaTest, "quick");

}
