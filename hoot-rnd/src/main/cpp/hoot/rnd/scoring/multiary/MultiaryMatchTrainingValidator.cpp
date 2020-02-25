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
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MultiaryMatchTrainingValidator.h"

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/schema/MetadataTags.h>

namespace hoot
{

/**
 * This is very specific to this class. No need to put it into its own file.
 */
class CheckRelationshipsVisitor : public ConstElementVisitor
{
public:

  virtual void visit(const ConstElementPtr& e)
  {
    const Tags& t = e->getTags();
    const QString REV = MetadataTags::TrainingReview();
    const QString MAT = MetadataTags::TrainingMatch();

    if (t.contains(MetadataTags::TrainingId()) ||
        t.contains(MAT) ||
        t.contains(REV))
    {
      if (t.contains(MetadataTags::TrainingId()) == false ||
          t.contains(MAT) == false)
      {
        LOG_WARN(QString("Element %1 doesn't contain %2 and %3.")
                 .arg(e->getElementId().toString())
                 .arg(MetadataTags::TrainingId())
                 .arg(MAT));
      }

      if (t[MAT] == "todo")
      {
        LOG_WARN("Element " << e->getElementId().toString() << " (" <<
                 t[MetadataTags::TrainingId()] << ") is still marked as todo: ");
      }
      if (!(t[MAT] == "none" || t[MAT] == "") &&
          !(t[REV] == "none" || t[REV] == ""))
      {
        LOG_WARN("Element " << e->getElementId().toString() << " (" <<
                 t[MetadataTags::TrainingId()] << ") has both match and review populated.");
        LOG_WARN("    '" << t[MAT] << "' and '" <<
                 t[REV] << "'");
      }
    }
  }

  virtual QString getDescription() const { return ""; }
  virtual std::string getClassName() const { return ""; }
};


MultiaryMatchTrainingValidator::MultiaryMatchTrainingValidator()
{
}

void MultiaryMatchTrainingValidator::apply(std::shared_ptr<OsmMap>& map)
{
  CheckRelationshipsVisitor v;
  map->visitRo(v);
}

}
