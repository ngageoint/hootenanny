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
package hoot.services.readers.review;

import java.sql.Connection;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.geo.BoundingBox;
import hoot.services.models.review.AllReviewableItems;
import hoot.services.models.review.ReviewableItem;
import hoot.services.models.review.ReviewableStatistics;


public class ReviewableReader {
    private static final Logger logger = LoggerFactory.getLogger(ReviewableReader.class);

    private final ReviewableQueryFactory fac = new ReviewableQueryFactory();
    private final Connection conn;

    public ReviewableReader(Connection conn) {
        this.conn = conn;
    }

    public ReviewableItem getReviewableItem(long mapid, long seqid) throws Exception {
        ReviewableItem r = (ReviewableItem) fac.getReviewableQuery(conn, mapid, seqid).execQuery();
        return r;
    }

    public ReviewableItem getRandomReviewableItem(long mapid) throws Exception {
        ReviewableItem r = (ReviewableItem) fac.getRandomReviewableQuery(conn, mapid).execQuery();
        return r;
    }

    public ReviewableStatistics getReviewablesStatistics(long mapid) throws Exception {
        ReviewableStatistics r = (ReviewableStatistics) fac.getReviewableStatisticsQuery(conn, mapid).execQuery();
        return r;
    }

    public AllReviewableItems getAllReviewableItems(long mapid, BoundingBox bbox) throws Exception {
        AllReviewableItems r = (AllReviewableItems) fac.getAllReviewableItemsQuery(conn, mapid, bbox).execQuery();
        return r;
    }
}
