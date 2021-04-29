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

import com.querydsl.sql.ColumnMetadata;
import com.querydsl.core.types.dsl.*;
import com.querydsl.core.types.PathMetadata;
import com.querydsl.core.types.Path;

import javax.annotation.Generated;

import java.sql.Types;


/**
 * QTranslationFolders is a Querydsl query type for Folders
 */
@Generated("com.querydsl.sql.codegen.MetaDataSerializer")
public class QTranslationFolders extends com.querydsl.sql.RelationalPathBase<TranslationFolder> {

    private static final long serialVersionUID = -268403029;

    public static final QTranslationFolders translationFolders = new QTranslationFolders("translation_folders");

    public final NumberPath<Long> id = createNumber("id", Long.class);

    public final StringPath displayName = createString("displayName");

    public final NumberPath<Long> parentId = createNumber("parentId", Long.class);

    public final NumberPath<Long> userId = createNumber("userId", Long.class);

    public final BooleanPath publicCol = createBoolean("publicCol");

    public final DateTimePath<java.sql.Timestamp> createdAt = createDateTime("createdAt", java.sql.Timestamp.class);

    public final StringPath path = createString("path");

    public final com.querydsl.sql.PrimaryKey<TranslationFolder> foldersPk = createPrimaryKey(id);

    public QTranslationFolders(String variable) {
        super(TranslationFolder.class, forVariable(variable), "public", "translation_folders");
        addMetadata();
    }

    public QTranslationFolders(String variable, String schema, String table) {
        super(TranslationFolder.class, forVariable(variable), schema, table);
        addMetadata();
    }

    public QTranslationFolders(Path<? extends TranslationFolder> path) {
        super(path.getType(), path.getMetadata(), "public", "translation_folders");
        addMetadata();
    }

    public QTranslationFolders(PathMetadata metadata) {
        super(TranslationFolder.class, metadata, "public", "translation_folders");
        addMetadata();
    }

    public void addMetadata() {
        addMetadata(id, ColumnMetadata.named("id").withIndex(1).ofType(Types.BIGINT).withSize(19).notNull());
        addMetadata(displayName, ColumnMetadata.named("display_name").withIndex(2).ofType(Types.VARCHAR).withSize(255).notNull());
        addMetadata(parentId, ColumnMetadata.named("parent_id").withIndex(3).ofType(Types.BIGINT).withSize(19));
        addMetadata(userId, ColumnMetadata.named("user_id").withIndex(4).ofType(Types.BIGINT).withSize(19).notNull());
        addMetadata(publicCol, ColumnMetadata.named("public").withIndex(5).ofType(Types.BIT).withSize(1).notNull());
        addMetadata(createdAt, ColumnMetadata.named("created_at").withIndex(6).ofType(Types.TIMESTAMP).withSize(35).withDigits(6).notNull());
        addMetadata(path, ColumnMetadata.named("path").withIndex(7).ofType(Types.VARCHAR).withSize(255).notNull());
    }

}

