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
package hoot.services.models.review;

import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


/**
 * 
 */
public class ReviewableStatistics extends ReviewQueryMapper {
    @SuppressWarnings("unused")
    private static final Logger log = LoggerFactory.getLogger(ReviewableStatistics.class);
    private long _totalCount = 0;
    private long _unreviewedCount = 0;

    public ReviewableStatistics(final long nTotal, final long nUnreviewed) {
        _totalCount = nTotal;
        _unreviewedCount = nUnreviewed;
    }

    public long getTotalCount() {
        return _totalCount;
    }

    public long getUnreviewedCount() {
        return _unreviewedCount;
    }

    @Override
    public String toString() {
        JSONObject o = new JSONObject();
        o.put("totalCount", _totalCount);
        o.put("unreviewedCount", _unreviewedCount);
        return o.toJSONString();
    }
}
