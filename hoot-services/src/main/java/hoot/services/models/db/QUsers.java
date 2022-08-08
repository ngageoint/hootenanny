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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2016, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
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
import com.querydsl.core.types.dsl.StringPath;
import com.querydsl.sql.ColumnMetadata;




/**
 * QUsers is a Querydsl query type for Users
 */
@Generated("com.querydsl.sql.codegen.MetaDataSerializer")
public class QUsers extends com.querydsl.sql.RelationalPathBase<Users> {

    private static final long serialVersionUID = 1881840750;

    public static final QUsers users = new QUsers("users");

    public final StringPath email = createString("email");

    public final StringPath displayName = createString("displayName");

    public final NumberPath<Long> id = createNumber("id", Long.class);

    public final StringPath provider_access_key = createString("provider_access_key");

    public final StringPath provider_access_token = createString("provider_access_token");

    public final DateTimePath<java.sql.Timestamp> hootservices_last_authorize = createDateTime(
            "hootservices_last_authorize", java.sql.Timestamp.class);

    public final DateTimePath<java.sql.Timestamp> hootservices_created_at = createDateTime("hootservices_created_at",
            java.sql.Timestamp.class);

    public final DateTimePath<java.sql.Timestamp> provider_created_at = createDateTime("provider_created_at",
            java.sql.Timestamp.class);

    public final SimplePath<Object> privileges = createSimple("privileges", Object.class);

    public final SimplePath<Object> favoriteOpts = createSimple("favorite_opts", Object.class);

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
        addMetadata(email, ColumnMetadata.named("email").withIndex(1).ofType(Types.VARCHAR).withSize(255).notNull());
        addMetadata(id, ColumnMetadata.named("id").withIndex(2).ofType(Types.BIGINT).withSize(19).notNull());
        addMetadata(displayName,
                ColumnMetadata.named("display_name").withIndex(3).ofType(Types.VARCHAR).withSize(255).notNull());

        addMetadata(provider_access_key,
                ColumnMetadata.named("provider_access_key").withIndex(4).ofType(Types.VARCHAR).withSize(255).notNull());
        addMetadata(provider_access_token, ColumnMetadata.named("provider_access_token").withIndex(5)
                .ofType(Types.VARCHAR).withSize(255).notNull());
        addMetadata(hootservices_last_authorize, ColumnMetadata.named("hootservices_last_authorize").withIndex(6)
                .ofType(Types.TIMESTAMP).withSize(29).withDigits(6).notNull());
        addMetadata(hootservices_created_at, ColumnMetadata.named("hootservices_created_at").withIndex(7)
                .ofType(Types.TIMESTAMP).withSize(29).withDigits(6).notNull());
        addMetadata(provider_created_at, ColumnMetadata.named("provider_created_at").withIndex(8)
                .ofType(Types.TIMESTAMP).withSize(29).withDigits(6).notNull());
        addMetadata(privileges, ColumnMetadata.named("privileges").withIndex(9).ofType(Types.OTHER).withSize(2147483647));
        addMetadata(favoriteOpts, ColumnMetadata.named("favorite_opts").withIndex(10).ofType(Types.OTHER).withSize(2147483647));
    }

}


