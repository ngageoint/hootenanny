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
  static const QString HootTagPrefix()        { return "hoot:"; }

  static const QString Accuracy()             { return "accuracy"; }

  static const QString ErrorCircular()        { return "error:circular"; }

  static const QString HootBuildingMatch()    { return "hoot:building:match"; }

  static const QString HootActual()           { return "hoot:actual"; }
  static const QString HootExpected()         { return "hoot:expected"; }
  static const QString HootMismatch()         { return "hoot:mismatch"; }
  static const QString HootWrong()            { return "hoot:wrong"; }

  static const QString HootEdge()             { return "hoot:edge"; }
  static const QString HootEdgeId()           { return "hoot:edge:id"; }
  static const QString HootEdgeScore()        { return "hoot:edge:score"; }
  static const QString HootEdgeScore12()      { return "hoot:edge:score12"; }
  static const QString HootEdgeScore21()      { return "hoot:edge:score21"; }

  static const QString HootVertex()           { return "hoot:vertex"; }
  static const QString HootVertexScore()      { return "hoot:vertex:score"; }
  static const QString HootVertexScore12()    { return "hoot:vertex:score12"; }
  static const QString HootVertexScore21()    { return "hoot:vertex:score21"; }

  static const QString HootId()               { return "hoot:id"; }

  static const QString HootLayername()        { return "hoot:layername"; }

  static const QString HootMatchOrder()       { return "hoot:match:order"; }
  static const QString HootMatchP()           { return "hoot:match:p"; }
  static const QString HootMatchScore()       { return "hoot:match:score"; }

  static const QString HootPertied()          { return "hoot:pertied"; }

  static const QString HootReviewChoices()    { return "hoot:review:choices"; }
  static const QString HootReviewMembers()    { return "hoot:review:members"; }
  static const QString HootReviewNeeds()      { return "hoot:review:needs"; }
  static const QString HootReviewNote()       { return "hoot:review:note"; }
  static const QString HootReviewScore()      { return "hoot:review:score"; }
  static const QString HootReviewSortOrder()  { return "hoot:review:sort_order"; }
  static const QString HootReviewType()       { return "hoot:review:type"; }
  static const QString HootReviewUuid()       { return "hoot:review:uuid"; }

  static const QString HootScoreMatch()       { return "hoot:score:match"; }
  static const QString HootScoreMiss()        { return "hoot:score:miss"; }
  static const QString HootScoreReview()      { return "hoot:score:review"; }
  static const QString HootScoreUuid()        { return "hoot:score:uuid"; }

  static const QString HootStatus()           { return "hoot:status"; }

  static const QString HootStub()             { return "hoot:stub"; }

  static const QString Ref1()                 { return "REF1"; }
  static const QString Ref2()                 { return "REF2"; }

  static const QString Unknown1()             { return "Unknown1"; }
  static const QString Unknown2()             { return "Unknown2"; }

private:

  MetadataTags() { }
};

}

#endif // METADATA_TAGS_H
