package hoot.services.models.db;

import static com.mysema.query.types.PathMetadataFactory.*;

import com.mysema.query.types.path.*;

import com.mysema.query.types.PathMetadata;
import javax.annotation.Generated;
import com.mysema.query.types.Path;

import com.mysema.query.sql.ColumnMetadata;




/**
 * QFolderMapMappings is a Querydsl query type for FolderMapMappings
 */
@Generated("com.mysema.query.sql.codegen.MetaDataSerializer")
public class QFolderMapMappings extends com.mysema.query.sql.RelationalPathBase<FolderMapMappings> {

    private static final long serialVersionUID = 1476505337;

    public static final QFolderMapMappings folderMapMappings = new QFolderMapMappings("folder_map_mappings");

    public final NumberPath<Long> folderId = createNumber("folderId", Long.class);

    public final NumberPath<Long> id = createNumber("id", Long.class);

    public final NumberPath<Long> mapId = createNumber("mapId", Long.class);

    public final com.mysema.query.sql.PrimaryKey<FolderMapMappings> folderMapMappingsPk = createPrimaryKey(id);

    public QFolderMapMappings(String variable) {
        super(FolderMapMappings.class, forVariable(variable), "public", "folder_map_mappings");
        addMetadata();
    }

    public QFolderMapMappings(String variable, String schema, String table) {
        super(FolderMapMappings.class, forVariable(variable), schema, table);
        addMetadata();
    }

    public QFolderMapMappings(Path<? extends FolderMapMappings> path) {
        super(path.getType(), path.getMetadata(), "public", "folder_map_mappings");
        addMetadata();
    }

    public QFolderMapMappings(PathMetadata<?> metadata) {
        super(FolderMapMappings.class, metadata, "public", "folder_map_mappings");
        addMetadata();
    }

    public void addMetadata() {
        addMetadata(folderId, ColumnMetadata.named("folder_id").ofType(-5).withSize(19).notNull());
        addMetadata(id, ColumnMetadata.named("id").ofType(-5).withSize(19).notNull());
        addMetadata(mapId, ColumnMetadata.named("map_id").ofType(-5).withSize(19).notNull());
    }

}

