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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef NOINFORMATIONCRITERION_H
#define NOINFORMATIONCRITERION_H

// hoot
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/elements/Tags.h>
#include <hoot/core/util/MetadataTags.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/Element.h>

#include "ElementCriterion.h"

namespace hoot
{

/**
 * A filter that will remove elements with no non-metadata information.
 */
class NoInformationCriterion : public ElementCriterion, public Configurable
{
public:

  static string className() { return "hoot::NoInformationCriterion"; }

  NoInformationCriterion() { setConfiguration(conf()); }
  NoInformationCriterion(bool treatReviewTagsAsMetadata) :
    _treatReviewTagsAsMetadata(treatReviewTagsAsMetadata) { }

  bool isSatisfied(const shared_ptr<const Element> &e) const
  {
    const Tags tags = e->getTags();
    const int informationCount = tags.getInformationCount();
    const int reviewTagCount =
      tags.getList("regex?" + MetadataTags::HootReviewTagPrefix() + ".*").size();

    LOG_VART(e);
    LOG_VART(informationCount);
    LOG_VART(_treatReviewTagsAsMetadata);
    LOG_VART(reviewTagCount);

    bool isSatisified = informationCount == 0;
    if (!_treatReviewTagsAsMetadata)
    {
      isSatisified &= reviewTagCount == 0;
    }
    LOG_VART(isSatisified);
    return isSatisified;
  }

  virtual void setConfiguration(const Settings& conf)
  {
    _treatReviewTagsAsMetadata = ConfigOptions(conf).getReviewTagsTreatAsMetadata();
    //LOG_VART(_treatReviewTagsAsMetadata);
  }

  virtual ElementCriterion* clone() { return new NoInformationCriterion(_treatReviewTagsAsMetadata); }

private:

  bool _treatReviewTagsAsMetadata;

};

}

#endif // NOINFORMATIONCRITERION_H
