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

import com.querydsl.core.types.PathMetadata;
import javax.annotation.Generated;
import com.querydsl.core.types.Path;

import com.querydsl.sql.ColumnMetadata;
import com.querydsl.sql.PrimaryKey;
import com.querydsl.sql.RelationalPathBase;

import java.sql.Types;


/**
 * QTranslationFolderMappings is a Querydsl query type for TranslationFolderMappings
 */
@Generated("com.querydsl.sql.codegen.MetaDataSerializer")
public class QTranslationFolderMappings extends RelationalPathBase<TranslationFolderMappings> {

    private static final long serialVersionUID = 1476505337;

    public static final QTranslationFolderMappings translationFolderMappings = new QTranslationFolderMappings("translation_folder_mappings");

    public final NumberPath<Long> id = createNumber("id", Long.class);

    public final StringPath displayName = createString("displayName");

    public final NumberPath<Long> folderId = createNumber("folderId", Long.class);

    public final PrimaryKey<TranslationFolderMappings> translationFolderMappingsPk = createPrimaryKey(id);

    public QTranslationFolderMappings(String variable) {
        super(TranslationFolderMappings.class, forVariable(variable), "public", "translation_folder_mappings");
        addMetadata();
    }

    public QTranslationFolderMappings(String variable, String schema, String table) {
        super(TranslationFolderMappings.class, forVariable(variable), schema, table);
        addMetadata();
    }

    public QTranslationFolderMappings(Path<? extends TranslationFolderMappings> path) {
        super(path.getType(), path.getMetadata(), "public", "translation_folder_mappings");
        addMetadata();
    }

    public QTranslationFolderMappings(PathMetadata metadata) {
        super(TranslationFolderMappings.class, metadata, "public", "translation_folder_mappings");
        addMetadata();
    }

    public void addMetadata() {
        addMetadata(id, ColumnMetadata.named("id").withIndex(1).ofType(Types.BIGINT).withSize(19).notNull());
        addMetadata(displayName, ColumnMetadata.named("display_name").withIndex(2).ofType(Types.VARCHAR).withSize(255).notNull());
        addMetadata(folderId, ColumnMetadata.named("folder_id").withIndex(3).ofType(Types.BIGINT).withSize(19).notNull());
    }

}

