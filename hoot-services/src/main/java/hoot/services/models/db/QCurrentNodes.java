package hoot.services.models.db;

import static com.mysema.query.types.PathMetadataFactory.*;

import com.mysema.query.types.path.*;

import com.mysema.query.types.PathMetadata;
import javax.annotation.Generated;
import com.mysema.query.types.Path;

import com.mysema.query.sql.ColumnMetadata;




/**
 * QCurrentNodes is a Querydsl query type for CurrentNodes
 */
@Generated("com.mysema.query.sql.codegen.MetaDataSerializer")
public class QCurrentNodes extends com.mysema.query.sql.RelationalPathBase<CurrentNodes> {

    private static final long serialVersionUID = -1530659950;

    public static final QCurrentNodes currentNodes = new QCurrentNodes("current_nodes");

    public final NumberPath<Long> changesetId = createNumber("changesetId", Long.class);

    public final NumberPath<Long> id = createNumber("id", Long.class);

    public final NumberPath<Double> latitude = createNumber("latitude", Double.class);

    public final NumberPath<Double> longitude = createNumber("longitude", Double.class);

    public final SimplePath<Object> tags = createSimple("tags", Object.class);

    public final NumberPath<Long> tile = createNumber("tile", Long.class);

    public final DateTimePath<java.sql.Timestamp> timestamp = createDateTime("timestamp", java.sql.Timestamp.class);

    public final NumberPath<Long> version = createNumber("version", Long.class);

    public final BooleanPath visible = createBoolean("visible");

    public final com.mysema.query.sql.PrimaryKey<CurrentNodes> currentNodesPkey = createPrimaryKey(id);

    public QCurrentNodes(String variable) {
        super(CurrentNodes.class, forVariable(variable), "public", "current_nodes");
        addMetadata();
    }

    public QCurrentNodes(String variable, String schema, String table) {
        super(CurrentNodes.class, forVariable(variable), schema, table);
        addMetadata();
    }

    public QCurrentNodes(Path<? extends CurrentNodes> path) {
        super(path.getType(), path.getMetadata(), "public", "current_nodes");
        addMetadata();
    }

    public QCurrentNodes(PathMetadata<?> metadata) {
        super(CurrentNodes.class, metadata, "public", "current_nodes");
        addMetadata();
    }

    public void addMetadata() {
        addMetadata(changesetId, ColumnMetadata.named("changeset_id").ofType(-5).withSize(19).notNull());
        addMetadata(id, ColumnMetadata.named("id").ofType(-5).withSize(19).notNull());
        addMetadata(latitude, ColumnMetadata.named("latitude").ofType(8).withSize(17).withDigits(17).notNull());
        addMetadata(longitude, ColumnMetadata.named("longitude").ofType(8).withSize(17).withDigits(17).notNull());
        addMetadata(tags, ColumnMetadata.named("tags").ofType(1111).withSize(2147483647));
        addMetadata(tile, ColumnMetadata.named("tile").ofType(-5).withSize(19).notNull());
        addMetadata(timestamp, ColumnMetadata.named("timestamp").ofType(93).withSize(29).withDigits(6).notNull());
        addMetadata(version, ColumnMetadata.named("version").ofType(-5).withSize(19).notNull());
        addMetadata(visible, ColumnMetadata.named("visible").ofType(-7).withSize(1).notNull());
    }

}

