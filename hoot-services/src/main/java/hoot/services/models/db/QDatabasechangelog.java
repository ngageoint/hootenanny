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
 * QDatabasechangelog is a Querydsl query type for Databasechangelog
 */
@Generated("com.querydsl.sql.codegen.MetaDataSerializer")
public class QDatabasechangelog extends com.querydsl.sql.RelationalPathBase<Databasechangelog> {

    private static final long serialVersionUID = -1877688353;

    public static final QDatabasechangelog databasechangelog = new QDatabasechangelog("databasechangelog");

    public final StringPath author = createString("author");

    public final StringPath comments = createString("comments");

    public final DateTimePath<java.sql.Timestamp> dateexecuted = createDateTime("dateexecuted", java.sql.Timestamp.class);

    public final StringPath description = createString("description");

    public final StringPath exectype = createString("exectype");

    public final StringPath filename = createString("filename");

    public final StringPath id = createString("id");

    public final StringPath liquibase = createString("liquibase");

    public final StringPath md5sum = createString("md5sum");

    public final NumberPath<Integer> orderexecuted = createNumber("orderexecuted", Integer.class);

    public final StringPath tag = createString("tag");

    public QDatabasechangelog(String variable) {
        super(Databasechangelog.class, forVariable(variable), "public", "databasechangelog");
        addMetadata();
    }

    public QDatabasechangelog(String variable, String schema, String table) {
        super(Databasechangelog.class, forVariable(variable), schema, table);
        addMetadata();
    }

    public QDatabasechangelog(Path<? extends Databasechangelog> path) {
        super(path.getType(), path.getMetadata(), "public", "databasechangelog");
        addMetadata();
    }

    public QDatabasechangelog(PathMetadata metadata) {
        super(Databasechangelog.class, metadata, "public", "databasechangelog");
        addMetadata();
    }

    public void addMetadata() {
        addMetadata(author, ColumnMetadata.named("author").withIndex(2).ofType(Types.VARCHAR).withSize(255).notNull());
        addMetadata(comments, ColumnMetadata.named("comments").withIndex(9).ofType(Types.VARCHAR).withSize(255));
        addMetadata(dateexecuted, ColumnMetadata.named("dateexecuted").withIndex(4).ofType(Types.TIMESTAMP).withSize(35).withDigits(6).notNull());
        addMetadata(description, ColumnMetadata.named("description").withIndex(8).ofType(Types.VARCHAR).withSize(255));
        addMetadata(exectype, ColumnMetadata.named("exectype").withIndex(6).ofType(Types.VARCHAR).withSize(10).notNull());
        addMetadata(filename, ColumnMetadata.named("filename").withIndex(3).ofType(Types.VARCHAR).withSize(255).notNull());
        addMetadata(id, ColumnMetadata.named("id").withIndex(1).ofType(Types.VARCHAR).withSize(255).notNull());
        addMetadata(liquibase, ColumnMetadata.named("liquibase").withIndex(11).ofType(Types.VARCHAR).withSize(20));
        addMetadata(md5sum, ColumnMetadata.named("md5sum").withIndex(7).ofType(Types.VARCHAR).withSize(35));
        addMetadata(orderexecuted, ColumnMetadata.named("orderexecuted").withIndex(5).ofType(Types.INTEGER).withSize(10).notNull());
        addMetadata(tag, ColumnMetadata.named("tag").withIndex(10).ofType(Types.VARCHAR).withSize(255));
    }

}

