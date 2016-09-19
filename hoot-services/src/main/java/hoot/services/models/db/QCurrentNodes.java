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

import static com.querydsl.core.types.PathMetadataFactory.*;

import com.querydsl.core.types.dsl.*;

import com.querydsl.core.types.PathMetadata;
import javax.annotation.Generated;
import com.querydsl.core.types.Path;

import com.querydsl.sql.ColumnMetadata;
import java.sql.Types;




/**
 * QCurrentNodes is a Querydsl query type for CurrentNodes
 */
@Generated("com.querydsl.sql.codegen.MetaDataSerializer")
public class QCurrentNodes extends com.querydsl.sql.RelationalPathBase<CurrentNodes> {

    private static final long serialVersionUID = -1530659950;

    public static final QCurrentNodes currentNodes = new QCurrentNodes("current_nodes");

    public final NumberPath<Long> changesetId = createNumber("changesetId", Long.class);

    public final NumberPath<Long> id = createNumber("id", Long.class);

    public final NumberPath<Double> latitude = createNumber("latitude", Double.class);

    public final NumberPath<Double> longitude = createNumber("longitude", Double.class);

    public final SimplePath<Object> tags = createSimple("tags", Object.class);

    public final NumberPath<Long> tile = createNumber("tile", Long.class);

    public final DateTimePath<java.sql.Timestamp> timestamp = createDateTime("timestamp", java.sql.Timestamp.class);

    public final NumberPath<Long> version = createNumber("version", Long.class);

    public final BooleanPath visible = createBoolean("visible");

    public final com.querydsl.sql.PrimaryKey<CurrentNodes> currentNodesPkey = createPrimaryKey(id);

    public QCurrentNodes(String variable) {
        super(CurrentNodes.class, forVariable(variable), "public", "current_nodes");
        addMetadata();
    }

    public QCurrentNodes(String variable, String schema, String table) {
        super(CurrentNodes.class, forVariable(variable), schema, table);
        addMetadata();
    }

    public QCurrentNodes(Path<? extends CurrentNodes> path) {
        super(path.getType(), path.getMetadata(), "public", "current_nodes");
        addMetadata();
    }

    public QCurrentNodes(PathMetadata metadata) {
        super(CurrentNodes.class, metadata, "public", "current_nodes");
        addMetadata();
    }

    public void addMetadata() {
        addMetadata(changesetId, ColumnMetadata.named("changeset_id").withIndex(4).ofType(Types.BIGINT).withSize(19).notNull());
        addMetadata(id, ColumnMetadata.named("id").withIndex(1).ofType(Types.BIGINT).withSize(19).notNull());
        addMetadata(latitude, ColumnMetadata.named("latitude").withIndex(2).ofType(Types.DOUBLE).withSize(17).withDigits(17).notNull());
        addMetadata(longitude, ColumnMetadata.named("longitude").withIndex(3).ofType(Types.DOUBLE).withSize(17).withDigits(17).notNull());
        addMetadata(tags, ColumnMetadata.named("tags").withIndex(9).ofType(Types.OTHER).withSize(2147483647));
        addMetadata(tile, ColumnMetadata.named("tile").withIndex(7).ofType(Types.BIGINT).withSize(19).notNull());
        addMetadata(timestamp, ColumnMetadata.named("timestamp").withIndex(6).ofType(Types.TIMESTAMP).withSize(29).withDigits(6).notNull());
        addMetadata(version, ColumnMetadata.named("version").withIndex(8).ofType(Types.BIGINT).withSize(19).notNull());
        addMetadata(visible, ColumnMetadata.named("visible").withIndex(5).ofType(Types.BIT).withSize(1).notNull());
    }

}

