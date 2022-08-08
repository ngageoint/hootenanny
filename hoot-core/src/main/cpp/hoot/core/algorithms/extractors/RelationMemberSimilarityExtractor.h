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
 * @copyright Copyright (C) 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef RELATION_MEMBER_SIMILARITY_EXTRACTOR_H
#define RELATION_MEMBER_SIMILARITY_EXTRACTOR_H

// hoot
#include <hoot/core/algorithms/extractors/FeatureExtractorBase.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * @brief The RelationMemberSimilarityExtractor class scores how similar the members of two
 * relations are.
 *
 * It uses a Jaccard index which calculates a score in the range of 0.0 to 1.0. Since relations may
 * have similar members with different element IDs between reference and secondary layers, the
 * comparisons ignore element IDs and look at the member elements directly.
 *
 * https://en.wikipedia.org/wiki/Jaccard_index
 * https://www.statisticshowto.datasciencecentral.com/jaccard-index/
 */
class RelationMemberSimilarityExtractor : public FeatureExtractorBase
{
public:

  RelationMemberSimilarityExtractor() = default;
  ~RelationMemberSimilarityExtractor() = default;

  static QString className() { return "hoot::RelationMemberSimilarityExtractor"; }

  double extract(const OsmMap& map, const std::shared_ptr<const Element>& target,
    const std::shared_ptr<const Element>& candidate) const override;

  QString getClassName() const override { return className(); }
  QString getDescription() const override
  { return "Scores the similarity between two sets of relation members"; }
  QString getName() const override { return className(); }
};

}

#endif // RELATION_MEMBER_SIMILARITY_EXTRACTOR_H
