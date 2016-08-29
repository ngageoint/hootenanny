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
import hoot.services.models.review.AllReviewableItems;
import hoot.services.models.review.ReviewableItem;
import hoot.services.models.review.ReviewableStatistics;


public class ReviewableReader {
    private final Connection connection;

    public ReviewableReader(Connection connection) {
        this.connection = connection;
    }

    public ReviewableItem getReviewableItem(long mapId, long sortOrder) {
        return (ReviewableItem) ReviewableQueryFactory.getReviewableQuery(connection, mapId, sortOrder).execQuery();
    }

    public ReviewableItem getRandomReviewableItem(long mapId) {
        return (ReviewableItem) ReviewableQueryFactory.getRandomReviewableQuery(connection, mapId).execQuery();
    }

    public ReviewableStatistics getReviewablesStatistics(long mapId) {
        return (ReviewableStatistics) ReviewableQueryFactory.getReviewableStatisticsQuery(connection, mapId).execQuery();
    }

    public AllReviewableItems getAllReviewableItems(long mapId, BoundingBox bbox) {
        return (AllReviewableItems) ReviewableQueryFactory.getAllReviewableItemsQuery(connection, mapId, bbox).execQuery();
    }
}
