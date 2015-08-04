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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ReviewMarker.h"

#include <hoot/core/util/Log.h>

namespace hoot
{

ReviewMarker::ReviewMarker()
{
}

bool ReviewMarker::isNeedsReview(const Tags& tags)
{
  return tags.isTrue(reviewNeedsKey());
}

bool ReviewMarker::isNeedsReview(ConstElementPtr e1, ConstElementPtr e2)
{
  return isNeedsReview(e1->getTags()) && isNeedsReview(e2->getTags()) &&
    (e1->getTags().get(reviewUuidKey()).contains(e2->getTags().get("uuid")) ||
     e2->getTags().get(reviewUuidKey()).contains(e1->getTags().get("uuid")));
}

void ReviewMarker::mark(ElementPtr& e1, ElementPtr& e2, const QString& note, double score)
{
  markElement(e1, e2, note, score);
  markElement(e2, e1, note, score);
}

void ReviewMarker::mark(ElementPtr& e, const QString& note, double score)
{
  e->getTags().set(reviewNeedsKey(), true);
  e->getTags().set(reviewSourceKey(), QString::number(e->getStatus().getEnum()));
  if (note.isEmpty())
  {
    throw IllegalArgumentException("You must specify a review note.");
  }
  e->getTags().appendValueIfUnique(reviewNoteKey(), note);
  _updateScore(e->getTags(), score);
}

void ReviewMarker::markElement(ElementPtr& e, ElementPtr& other, const QString& note, double score)
{
  e->getTags().appendValueIfUnique(reviewUuidKey(), other->getTags().getCreateUuid());
  e->getTags().set(reviewNeedsKey(), true);
  e->getTags().set(reviewSourceKey(), QString::number(e->getStatus().getEnum()));
  if (note.isEmpty())
  {
    throw IllegalArgumentException("You must specify a review note.");
  }
  e->getTags().appendValueIfUnique(reviewNoteKey(), note);
  _updateScore(e->getTags(), score);
}

void ReviewMarker::_updateScore(Tags& t, double score)
{
  if (score >= 0)
  {
    QStringList l = t.getList(reviewScoreKey());

    if (l.size() > 1)
    {
      LOG_WARN("Found an unexpected list of scores. Did you forget to strip reviews "
               "before running conflate (-C RemoveReview2Pre.conf)? " + t.get(reviewScoreKey()));
    }

    double maxScore = score;
    for (int i = 0; i < l.size(); i++)
    {
      bool ok;
      double s = l[i].toDouble(&ok);
      if (ok)
      {
        maxScore = max(s, maxScore);
      }
      else
      {
        LOG_WARN("Found an invalid score in tags: " + t.toString());
      }
    }

    t.set(reviewScoreKey(), maxScore);
  }
}

}
