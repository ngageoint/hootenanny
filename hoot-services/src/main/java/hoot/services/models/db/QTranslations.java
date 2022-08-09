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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */
package hoot.services.models.db;

import static com.querydsl.core.types.PathMetadataFactory.*;

import com.querydsl.core.types.dsl.*;
import com.querydsl.core.types.Path;
import com.querydsl.core.types.PathMetadata;

import com.querydsl.sql.ColumnMetadata;
import com.querydsl.sql.PrimaryKey;
import com.querydsl.sql.RelationalPathBase;

import java.sql.Timestamp;
import java.sql.Types;

import javax.annotation.Generated;


/**
 * QTranslations is a Querydsl query type for Translations
 */
@Generated("com.querydsl.sql.codegen.MetaDataSerializer")
public class QTranslations extends RelationalPathBase<Translations> {

    private static final long serialVersionUID = 1476505337;

    public static final QTranslations translations = new QTranslations("translations");

    public final NumberPath<Long> id = createNumber("id", Long.class);

    public final StringPath displayName = createString("displayName");

    public final NumberPath<Long> userId = createNumber("userId", Long.class);

    public final BooleanPath publicCol = createBoolean("publicCol");

    public final DateTimePath<Timestamp> createdAt = createDateTime("createdAt", Timestamp.class);

    public final NumberPath<Long> folderId = createNumber("folderId", Long.class);

    public final PrimaryKey<Translations> translationsPk = createPrimaryKey(id);

    public QTranslations(String variable) {
        super(Translations.class, forVariable(variable), "public", "translations");
        addMetadata();
    }

    public QTranslations(String variable, String schema, String table) {
        super(Translations.class, forVariable(variable), schema, table);
        addMetadata();
    }

    public QTranslations(Path<? extends Translations> path) {
        super(path.getType(), path.getMetadata(), "public", "translations");
        addMetadata();
    }

    public QTranslations(PathMetadata metadata) {
        super(Translations.class, metadata, "public", "translations");
        addMetadata();
    }

    public void addMetadata() {
        addMetadata(id, ColumnMetadata.named("id").withIndex(1).ofType(Types.BIGINT).withSize(19).notNull());
        addMetadata(displayName, ColumnMetadata.named("display_name").withIndex(2).ofType(Types.VARCHAR).withSize(255).notNull());
        addMetadata(userId, ColumnMetadata.named("user_id").withIndex(3).ofType(Types.BIGINT).withSize(19).notNull());
        addMetadata(publicCol, ColumnMetadata.named("public").withIndex(4).ofType(Types.BIT).withSize(1).notNull());
        addMetadata(createdAt, ColumnMetadata.named("created_at").withIndex(5).ofType(Types.TIMESTAMP).withSize(29).withDigits(6).notNull());
        addMetadata(folderId, ColumnMetadata.named("folder_id").withIndex(6).ofType(Types.BIGINT).withSize(19).notNull());
    }

}

