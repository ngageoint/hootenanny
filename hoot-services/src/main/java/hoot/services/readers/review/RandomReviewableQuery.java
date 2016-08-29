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

import static hoot.services.HootProperties.RANDOM_QUERY_SEED;
import static hoot.services.HootProperties.SEED_RANDOM_QUERIES;
import static hoot.services.models.db.QCurrentRelations.currentRelations;

import java.sql.Connection;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.querydsl.core.Tuple;
import com.querydsl.core.types.dsl.Expressions;
import com.querydsl.core.types.dsl.NumberExpression;
import com.querydsl.sql.SQLQuery;

import hoot.services.models.review.ReviewQueryMapper;
import hoot.services.models.review.ReviewableItem;
import hoot.services.utils.DbUtils;


class RandomReviewableQuery extends ReviewableQueryBase implements IReviewableQuery {
    private static final Logger logger = LoggerFactory.getLogger(RandomReviewableQuery.class);

    RandomReviewableQuery(Connection connection, long mapid) {
        super(connection, mapid);

        // TODO: Since this code will affect all subsequent calls to
        // random(), it is better moved to
        // a more centralized location. Given that this is the only class
        // using random() in a SQL query so far, no harm is done for the time being.
        if (Boolean.parseBoolean(SEED_RANDOM_QUERIES)) {
            double seed = Double.parseDouble(RANDOM_QUERY_SEED);
            if ((seed >= -1.0) && (seed <= 1.0)) {
                new SQLQuery<>(super.getConnection(), DbUtils.getConfiguration())
                        .select(Expressions.numberTemplate(Double.class, "setseed(" + seed + ");"))
                        .from()
                        .fetch();
            }
        }
    }

    @Override
    public ReviewQueryMapper execQuery() {
        ReviewableItem reviewableItem = new ReviewableItem(-1, getMapId(), -1);

        Tuple result = new SQLQuery<Tuple>(this.getConnection(), DbUtils.getConfiguration(getMapId()))
                .select(currentRelations.id, Expressions.stringTemplate("tags->'hoot:review:sort_order'"))
                .from(currentRelations)
                .where(Expressions.booleanTemplate("tags->'hoot:review:needs' = 'yes'"))
                .orderBy(NumberExpression.random().asc())
                .limit(1)
                .fetchOne();

        if (result != null) {
            reviewableItem.setRelationId(result.get(0, Long.TYPE));
            String sortOrder = result.get(1, String.class);
            reviewableItem.setSortOrder((sortOrder != null) ? Long.parseLong(sortOrder) : -1L);
            reviewableItem.setResultCount(1L);
        }

        return reviewableItem;
    }
}
