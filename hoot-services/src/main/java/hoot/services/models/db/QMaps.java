package hoot.services.models.db;

import static com.mysema.query.types.PathMetadataFactory.*;

import com.mysema.query.types.path.*;

import com.mysema.query.types.PathMetadata;
import javax.annotation.Generated;
import com.mysema.query.types.Path;

import com.mysema.query.sql.ColumnMetadata;




/**
 * QMaps is a Querydsl query type for Maps
 */
@Generated("com.mysema.query.sql.codegen.MetaDataSerializer")
public class QMaps extends com.mysema.query.sql.RelationalPathBase<Maps> {

    private static final long serialVersionUID = 476091249;

    public static final QMaps maps = new QMaps("maps");

    public final DateTimePath<java.sql.Timestamp> createdAt = createDateTime("createdAt", java.sql.Timestamp.class);

    public final StringPath displayName = createString("displayName");

    public final NumberPath<Long> id = createNumber("id", Long.class);

    public final BooleanPath publicCol = createBoolean("publicCol");

    public final SimplePath<Object> tags = createSimple("tags", Object.class);

    public final NumberPath<Long> userId = createNumber("userId", Long.class);

    public final com.mysema.query.sql.PrimaryKey<Maps> mapsPk = createPrimaryKey(id);

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

    public QMaps(PathMetadata<?> metadata) {
        super(Maps.class, metadata, "public", "maps");
        addMetadata();
    }

    public void addMetadata() {
        addMetadata(createdAt, ColumnMetadata.named("created_at").ofType(93).withSize(29).withDigits(6).notNull());
        addMetadata(displayName, ColumnMetadata.named("display_name").ofType(12).withSize(255).notNull());
        addMetadata(id, ColumnMetadata.named("id").ofType(-5).withSize(19).notNull());
        addMetadata(publicCol, ColumnMetadata.named("public").ofType(-7).withSize(1).notNull());
        addMetadata(tags, ColumnMetadata.named("tags").ofType(1111).withSize(2147483647));
        addMetadata(userId, ColumnMetadata.named("user_id").ofType(-5).withSize(19).notNull());
    }

}

