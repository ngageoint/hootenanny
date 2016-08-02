package hoot.services.models.db;

import static com.mysema.query.types.PathMetadataFactory.*;

import com.mysema.query.types.path.*;

import com.mysema.query.types.PathMetadata;
import javax.annotation.Generated;
import com.mysema.query.types.Path;

import com.mysema.query.sql.ColumnMetadata;




/**
 * QFolders is a Querydsl query type for Folders
 */
@Generated("com.mysema.query.sql.codegen.MetaDataSerializer")
public class QFolders extends com.mysema.query.sql.RelationalPathBase<Folders> {

    private static final long serialVersionUID = -268403029;

    public static final QFolders folders = new QFolders("folders");

    public final DateTimePath<java.sql.Timestamp> createdAt = createDateTime("createdAt", java.sql.Timestamp.class);

    public final StringPath displayName = createString("displayName");

    public final NumberPath<Long> id = createNumber("id", Long.class);

    public final NumberPath<Long> parentId = createNumber("parentId", Long.class);

    public final BooleanPath publicCol = createBoolean("publicCol");

    public final NumberPath<Long> userId = createNumber("userId", Long.class);

    public final com.mysema.query.sql.PrimaryKey<Folders> foldersPk = createPrimaryKey(id);

    public QFolders(String variable) {
        super(Folders.class, forVariable(variable), "public", "folders");
        addMetadata();
    }

    public QFolders(String variable, String schema, String table) {
        super(Folders.class, forVariable(variable), schema, table);
        addMetadata();
    }

    public QFolders(Path<? extends Folders> path) {
        super(path.getType(), path.getMetadata(), "public", "folders");
        addMetadata();
    }

    public QFolders(PathMetadata<?> metadata) {
        super(Folders.class, metadata, "public", "folders");
        addMetadata();
    }

    public void addMetadata() {
        addMetadata(createdAt, ColumnMetadata.named("created_at").ofType(93).withSize(35).withDigits(6).notNull());
        addMetadata(displayName, ColumnMetadata.named("display_name").ofType(12).withSize(255).notNull());
        addMetadata(id, ColumnMetadata.named("id").ofType(-5).withSize(19).notNull());
        addMetadata(parentId, ColumnMetadata.named("parent_id").ofType(-5).withSize(19).notNull());
        addMetadata(publicCol, ColumnMetadata.named("public").ofType(-7).withSize(1).notNull());
        addMetadata(userId, ColumnMetadata.named("user_id").ofType(-5).withSize(19).notNull());
    }

}

