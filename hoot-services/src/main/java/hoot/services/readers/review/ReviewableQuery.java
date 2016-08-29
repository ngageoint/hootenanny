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

import static hoot.services.models.db.QCurrentRelations.currentRelations;

import java.sql.Connection;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.querydsl.core.types.dsl.Expressions;
import com.querydsl.sql.SQLQuery;

import hoot.services.models.review.ReviewQueryMapper;
import hoot.services.models.review.ReviewableItem;
import hoot.services.utils.DbUtils;


class ReviewableQuery extends ReviewableQueryBase implements IReviewableQuery {
    private static final Logger logger = LoggerFactory.getLogger(ReviewableQuery.class);
    private long sortOrder = -1;

    ReviewableQuery(Connection connection, long mapId, long sortOrder) {
        super(connection, mapId);
        this.sortOrder = sortOrder;
    }

    @Override
    public ReviewQueryMapper execQuery() {
        Long result = new SQLQuery<Long>(this.getConnection(), DbUtils.getConfiguration(getMapId()))
                .select(currentRelations.id)
                .from(currentRelations)
                .where(Expressions.booleanTemplate("tags->'hoot:review:needs' = 'yes'")
                        .and(Expressions.booleanTemplate("tags->'hoot:review:sort_order'='" + sortOrder + "'")))
                .fetchOne();

        ReviewableItem reviewableItem = new ReviewableItem(sortOrder, getMapId(), -1);

        if (result != null) {
            reviewableItem.setRelationId(result);
            reviewableItem.setResultCount(1);
        }

        return reviewableItem;
    }
}
