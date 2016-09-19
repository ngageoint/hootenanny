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
 * QDatabasechangeloglock is a Querydsl query type for Databasechangeloglock
 */
@Generated("com.querydsl.sql.codegen.MetaDataSerializer")
public class QDatabasechangeloglock extends com.querydsl.sql.RelationalPathBase<Databasechangeloglock> {

    private static final long serialVersionUID = -166298230;

    public static final QDatabasechangeloglock databasechangeloglock = new QDatabasechangeloglock("databasechangeloglock");

    public final NumberPath<Integer> id = createNumber("id", Integer.class);

    public final BooleanPath locked = createBoolean("locked");

    public final StringPath lockedby = createString("lockedby");

    public final DateTimePath<java.sql.Timestamp> lockgranted = createDateTime("lockgranted", java.sql.Timestamp.class);

    public final com.querydsl.sql.PrimaryKey<Databasechangeloglock> databasechangeloglockPk = createPrimaryKey(id);

    public QDatabasechangeloglock(String variable) {
        super(Databasechangeloglock.class, forVariable(variable), "public", "databasechangeloglock");
        addMetadata();
    }

    public QDatabasechangeloglock(String variable, String schema, String table) {
        super(Databasechangeloglock.class, forVariable(variable), schema, table);
        addMetadata();
    }

    public QDatabasechangeloglock(Path<? extends Databasechangeloglock> path) {
        super(path.getType(), path.getMetadata(), "public", "databasechangeloglock");
        addMetadata();
    }

    public QDatabasechangeloglock(PathMetadata metadata) {
        super(Databasechangeloglock.class, metadata, "public", "databasechangeloglock");
        addMetadata();
    }

    public void addMetadata() {
        addMetadata(id, ColumnMetadata.named("id").withIndex(1).ofType(Types.INTEGER).withSize(10).notNull());
        addMetadata(locked, ColumnMetadata.named("locked").withIndex(2).ofType(Types.BIT).withSize(1).notNull());
        addMetadata(lockedby, ColumnMetadata.named("lockedby").withIndex(4).ofType(Types.VARCHAR).withSize(255));
        addMetadata(lockgranted, ColumnMetadata.named("lockgranted").withIndex(3).ofType(Types.TIMESTAMP).withSize(35).withDigits(6));
    }

}

