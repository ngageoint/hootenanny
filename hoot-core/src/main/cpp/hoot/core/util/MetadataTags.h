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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef METADATA_TAGS_H
#define METADATA_TAGS_H

// Qt
#include <QString>

namespace hoot
{

class MetadataTags
{
public:
  /** Metadata tag names that are used throughout the code.  This class centralizes them
   *  into one place in the code base.  This class also includes a couple of key values too.
   */
  inline static const QString HootTagPrefix()         { return "hoot:"; }

  inline static const QString Accuracy()              { return "accuracy"; }
  inline static const QString ErrorCircular()         { return "error:circular"; }

  inline static const QString HootBuildingMatch()     { return "hoot:building:match"; }

  inline static const QString HootActual()            { return "hoot:actual"; }
  inline static const QString HootExpected()          { return "hoot:expected"; }
  inline static const QString HootMismatch()          { return "hoot:mismatch"; }
  inline static const QString HootWrong()             { return "hoot:wrong"; }

  inline static const QString HootEdge()              { return "hoot:edge"; }
  inline static const QString HootEdgeId()            { return "hoot:edge:id"; }
  inline static const QString HootEdgeScore()         { return "hoot:edge:score"; }
  inline static const QString HootEdgeScore12()       { return "hoot:edge:score12"; }
  inline static const QString HootEdgeScore21()       { return "hoot:edge:score21"; }

  inline static const QString HootVertex()            { return "hoot:vertex"; }
  inline static const QString HootVertexScore()       { return "hoot:vertex:score"; }
  inline static const QString HootVertexScore12()     { return "hoot:vertex:score12"; }
  inline static const QString HootVertexScore21()     { return "hoot:vertex:score21"; }

  inline static const QString HootId()                { return "hoot:id"; }

  inline static const QString HootLayername()         { return "hoot:layername"; }

  inline static const QString HootMatchOrder()        { return "hoot:match:order"; }
  inline static const QString HootMatchP()            { return "hoot:match:p"; }
  inline static const QString HootMatchScore()        { return "hoot:match:score"; }

  inline static const QString HootPertied()           { return "hoot:pertied"; }

  inline static const QString HootReviewTagPrefix()   { return "hoot:review:"; }
  inline static const QString HootReviewChoices()     { return "hoot:review:choices"; }
  inline static const QString HootReviewMembers()     { return "hoot:review:members"; }
  inline static const QString HootReviewNeeds()       { return "hoot:review:needs"; }
  inline static const QString HootReviewNote()        { return "hoot:review:note"; }
  inline static const QString HootReviewScore()       { return "hoot:review:score"; }
  inline static const QString HootReviewSortOrder()   { return "hoot:review:sort_order"; }
  inline static const QString HootReviewType()        { return "hoot:review:type"; }
  inline static const QString HootReviewUuid()        { return "hoot:review:uuid"; }

  inline static const QString HootScoreMatch()        { return "hoot:score:match"; }
  inline static const QString HootScoreMiss()         { return "hoot:score:miss"; }
  inline static const QString HootScoreReview()       { return "hoot:score:review"; }
  inline static const QString HootScoreUuid()         { return "hoot:score:uuid"; }

  inline static const QString HootStatus()            { return "hoot:status"; }
  inline static const QString HootSource()            { return "hoot:source"; }

  inline static const QString HootStub()              { return "hoot:stub"; }

  inline static const QString Ref1()                  { return "REF1"; }
  inline static const QString Ref2()                  { return "REF2"; }

  inline static const QString Unknown1()              { return "Unknown1"; }
  inline static const QString Unknown2()              { return "Unknown2"; }

private:

  MetadataTags() { }
};

}

#endif // METADATA_TAGS_H
