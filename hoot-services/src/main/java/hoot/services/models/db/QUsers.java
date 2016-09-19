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
 * QUsers is a Querydsl query type for Users
 */
@Generated("com.querydsl.sql.codegen.MetaDataSerializer")
public class QUsers extends com.querydsl.sql.RelationalPathBase<Users> {

    private static final long serialVersionUID = 1881840750;

    public static final QUsers users = new QUsers("users");

    public final StringPath displayName = createString("displayName");

    public final StringPath email = createString("email");

    public final NumberPath<Long> id = createNumber("id", Long.class);

    public final com.querydsl.sql.PrimaryKey<Users> usersPk = createPrimaryKey(id);

    public QUsers(String variable) {
        super(Users.class, forVariable(variable), "public", "users");
        addMetadata();
    }

    public QUsers(String variable, String schema, String table) {
        super(Users.class, forVariable(variable), schema, table);
        addMetadata();
    }

    public QUsers(Path<? extends Users> path) {
        super(path.getType(), path.getMetadata(), "public", "users");
        addMetadata();
    }

    public QUsers(PathMetadata metadata) {
        super(Users.class, metadata, "public", "users");
        addMetadata();
    }

    public void addMetadata() {
        addMetadata(displayName, ColumnMetadata.named("display_name").withIndex(3).ofType(Types.VARCHAR).withSize(255).notNull());
        addMetadata(email, ColumnMetadata.named("email").withIndex(1).ofType(Types.VARCHAR).withSize(255).notNull());
        addMetadata(id, ColumnMetadata.named("id").withIndex(2).ofType(Types.BIGINT).withSize(19).notNull());
    }

}

