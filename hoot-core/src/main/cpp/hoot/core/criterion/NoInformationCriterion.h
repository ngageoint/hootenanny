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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef NOINFORMATIONCRITERION_H
#define NOINFORMATIONCRITERION_H

// hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/schema/MetadataTags.h>

namespace hoot
{

/**
 * A criterion that will remove elements with no non-metadata information.
 */
class NoInformationCriterion : public ElementCriterion, public Configurable
{
public:

  static QString className() { return "NoInformationCriterion"; }

  NoInformationCriterion();
  NoInformationCriterion(bool treatReviewTagsAsMetadata);
  ~NoInformationCriterion() override = default;

  bool isSatisfied(const ConstElementPtr& e) const override;
  ElementCriterionPtr clone() override
  { return std::make_shared<NoInformationCriterion>(_treatReviewTagsAsMetadata); }
  void setConfiguration(const Settings& conf) override;

  QString getDescription() const override
  { return "Identifies features with no useful information"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString toString() const override { return className(); }

private:

  bool _treatReviewTagsAsMetadata;
};

}

#endif // NOINFORMATIONCRITERION_H
