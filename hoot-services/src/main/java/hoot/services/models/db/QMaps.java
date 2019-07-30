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
 * @copyright Copyright (C) 2016, 2019 DigitalGlobe (http://www.digitalglobe.com/)
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
 * QMaps is a Querydsl query type for Maps
 */
@Generated("com.querydsl.sql.codegen.MetaDataSerializer")
public class QMaps extends com.querydsl.sql.RelationalPathBase<Maps> {

    private static final long serialVersionUID = 476091249;

    public static final QMaps maps = new QMaps("maps");

    public final DateTimePath<java.sql.Timestamp> createdAt = createDateTime("createdAt", java.sql.Timestamp.class);

    public final StringPath displayName = createString("displayName");

    public final NumberPath<Long> id = createNumber("id", Long.class);

    public final BooleanPath publicCol = createBoolean("publicCol");

    public final SimplePath<Object> tags = createSimple("tags", Object.class);

    public final NumberPath<Long> userId = createNumber("userId", Long.class);

    public final com.querydsl.sql.PrimaryKey<Maps> mapsPk = createPrimaryKey(id);

    public QMaps(String variable) {
        super(Maps.class, forVariable(variable), "public", "maps");
        addMetadata();
    }

    public QMaps(String variable, String schema, String table) {
        super(Maps.class, forVariable(variable), schema, table);
        addMetadata();
    }

    public QMaps(Path<? extends Maps> path) {
        super(path.getType(), path.getMetadata(), "public", "maps");
        addMetadata();
    }

    public QMaps(PathMetadata metadata) {
        super(Maps.class, metadata, "public", "maps");
        addMetadata();
    }

    public void addMetadata() {
        addMetadata(id, ColumnMetadata.named("id").withIndex(1).ofType(Types.BIGINT).withSize(19).notNull());
        addMetadata(displayName, ColumnMetadata.named("display_name").withIndex(2).ofType(Types.VARCHAR).withSize(255).notNull());
        addMetadata(userId, ColumnMetadata.named("user_id").withIndex(3).ofType(Types.BIGINT).withSize(19).notNull());
        addMetadata(publicCol, ColumnMetadata.named("public").withIndex(4).ofType(Types.BIT).withSize(1).notNull());
        addMetadata(createdAt, ColumnMetadata.named("created_at").withIndex(5).ofType(Types.TIMESTAMP).withSize(29).withDigits(6).notNull());
        addMetadata(tags, ColumnMetadata.named("tags").withIndex(6).ofType(Types.OTHER).withSize(2147483647));
    }

}

