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

import hoot.services.geo.BoundingBox;


final class ReviewableQueryFactory {

    private ReviewableQueryFactory() {}

    static IReviewableQuery getReviewableQuery(Connection connection, long mapid, long seqid) {
        return new ReviewableQuery(connection, mapid, seqid);
    }

    static IReviewableQuery getRandomReviewableQuery(Connection connection, long mapid) {
        return new RandomReviewableQuery(connection, mapid);
    }

    static IReviewableQuery getReviewableStatisticsQuery(Connection connection, long mapid) {
        return new ReviewableStatisticsQuery(connection, mapid);
    }

    static IReviewableQuery getAllReviewableItemsQuery(Connection connection, long mapid, BoundingBox bbox) {
        return new AllReviewableItemsQuery(connection, mapid, bbox);
    }
}
