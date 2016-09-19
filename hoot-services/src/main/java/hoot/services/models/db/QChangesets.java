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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.models.db;

import static com.querydsl.core.types.PathMetadataFactory.forVariable;

import java.sql.Types;

import javax.annotation.Generated;

import com.querydsl.core.types.Path;
import com.querydsl.core.types.PathMetadata;
import com.querydsl.core.types.dsl.DateTimePath;
import com.querydsl.core.types.dsl.NumberPath;
import com.querydsl.core.types.dsl.SimplePath;
import com.querydsl.sql.ColumnMetadata;




/**
 * QChangesets is a Querydsl query type for Changesets
 */
@Generated("com.querydsl.sql.codegen.MetaDataSerializer")
public class QChangesets extends com.querydsl.sql.RelationalPathBase<Changesets> {

    private static final long serialVersionUID = -69300901;

    public static final QChangesets changesets = new QChangesets("changesets");

    public final DateTimePath<java.sql.Timestamp> closedAt = createDateTime("closedAt", java.sql.Timestamp.class);

    public final DateTimePath<java.sql.Timestamp> createdAt = createDateTime("createdAt", java.sql.Timestamp.class);

    public final NumberPath<Long> id = createNumber("id", Long.class);

    public final NumberPath<Double> maxLat = createNumber("maxLat", Double.class);

    public final NumberPath<Double> maxLon = createNumber("maxLon", Double.class);

    public final NumberPath<Double> minLat = createNumber("minLat", Double.class);

    public final NumberPath<Double> minLon = createNumber("minLon", Double.class);

    public final NumberPath<Integer> numChanges = createNumber("numChanges", Integer.class);

    public final SimplePath<Object> tags = createSimple("tags", Object.class);

    public final NumberPath<Long> userId = createNumber("userId", Long.class);

    public final com.querydsl.sql.PrimaryKey<Changesets> changesetsPk = createPrimaryKey(id);

    public QChangesets(String variable) {
        super(Changesets.class, forVariable(variable), "public", "changesets");
        addMetadata();
    }

    public QChangesets(String variable, String schema, String table) {
        super(Changesets.class, forVariable(variable), schema, table);
        addMetadata();
    }

    public QChangesets(Path<? extends Changesets> path) {
        super(path.getType(), path.getMetadata(), "public", "changesets");
        addMetadata();
    }

    public QChangesets(PathMetadata metadata) {
        super(Changesets.class, metadata, "public", "changesets");
        addMetadata();
    }

    public void addMetadata() {
        addMetadata(closedAt, ColumnMetadata.named("closed_at").withIndex(8).ofType(Types.TIMESTAMP).withSize(29).withDigits(6).notNull());
        addMetadata(createdAt, ColumnMetadata.named("created_at").withIndex(3).ofType(Types.TIMESTAMP).withSize(29).withDigits(6).notNull());
        addMetadata(id, ColumnMetadata.named("id").withIndex(1).ofType(Types.BIGINT).withSize(19).notNull());
        addMetadata(maxLat, ColumnMetadata.named("max_lat").withIndex(5).ofType(Types.DOUBLE).withSize(17).withDigits(17).notNull());
        addMetadata(maxLon, ColumnMetadata.named("max_lon").withIndex(7).ofType(Types.DOUBLE).withSize(17).withDigits(17).notNull());
        addMetadata(minLat, ColumnMetadata.named("min_lat").withIndex(4).ofType(Types.DOUBLE).withSize(17).withDigits(17).notNull());
        addMetadata(minLon, ColumnMetadata.named("min_lon").withIndex(6).ofType(Types.DOUBLE).withSize(17).withDigits(17).notNull());
        addMetadata(numChanges, ColumnMetadata.named("num_changes").withIndex(9).ofType(Types.INTEGER).withSize(10).notNull());
        addMetadata(tags, ColumnMetadata.named("tags").withIndex(10).ofType(Types.OTHER).withSize(2147483647));
        addMetadata(userId, ColumnMetadata.named("user_id").withIndex(2).ofType(Types.BIGINT).withSize(19).notNull());
    }

}

