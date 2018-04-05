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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef NOINFORMATIONCRITERION_H
#define NOINFORMATIONCRITERION_H

// hoot
#include <hoot/core/elements/Tags.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/MetadataTags.h>

#include "ElementCriterion.h"

namespace hoot
{

/**
 * A filter that will remove elements with no non-metadata information.
 */
class NoInformationCriterion : public ElementCriterion, public Configurable
{
public:

  static std::string className() { return "hoot::NoInformationCriterion"; }

  NoInformationCriterion() { setConfiguration(conf()); }
  NoInformationCriterion(bool treatReviewTagsAsMetadata) :
    _treatReviewTagsAsMetadata(treatReviewTagsAsMetadata) { }

  virtual bool isSatisfied(const boost::shared_ptr<const Element> &e) const;

  virtual void setConfiguration(const Settings& conf);

  virtual ElementCriterionPtr clone()
  { return ElementCriterionPtr(new NoInformationCriterion(_treatReviewTagsAsMetadata)); }

  virtual QString getDescription() const { return "Identifies features with no useful information"; }

private:

  bool _treatReviewTagsAsMetadata;

};

}

#endif // NOINFORMATIONCRITERION_H
