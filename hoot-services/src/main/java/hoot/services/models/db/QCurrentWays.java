package hoot.services.models.db;

import static com.mysema.query.types.PathMetadataFactory.*;

import com.mysema.query.types.path.*;

import com.mysema.query.types.PathMetadata;
import javax.annotation.Generated;
import com.mysema.query.types.Path;

import com.mysema.query.sql.ColumnMetadata;




/**
 * QCurrentWays is a Querydsl query type for CurrentWays
 */
@Generated("com.mysema.query.sql.codegen.MetaDataSerializer")
public class QCurrentWays extends com.mysema.query.sql.RelationalPathBase<CurrentWays> {

    private static final long serialVersionUID = 89426531;

    public static final QCurrentWays currentWays = new QCurrentWays("current_ways");

    public final NumberPath<Long> changesetId = createNumber("changesetId", Long.class);

    public final NumberPath<Long> id = createNumber("id", Long.class);

    public final SimplePath<Object> tags = createSimple("tags", Object.class);

    public final DateTimePath<java.sql.Timestamp> timestamp = createDateTime("timestamp", java.sql.Timestamp.class);

    public final NumberPath<Long> version = createNumber("version", Long.class);

    public final BooleanPath visible = createBoolean("visible");

    public final com.mysema.query.sql.PrimaryKey<CurrentWays> currentWaysPkey = createPrimaryKey(id);

    public QCurrentWays(String variable) {
        super(CurrentWays.class, forVariable(variable), "public", "current_ways");
        addMetadata();
    }

    public QCurrentWays(String variable, String schema, String table) {
        super(CurrentWays.class, forVariable(variable), schema, table);
        addMetadata();
    }

    public QCurrentWays(Path<? extends CurrentWays> path) {
        super(path.getType(), path.getMetadata(), "public", "current_ways");
        addMetadata();
    }

    public QCurrentWays(PathMetadata<?> metadata) {
        super(CurrentWays.class, metadata, "public", "current_ways");
        addMetadata();
    }

    public void addMetadata() {
        addMetadata(changesetId, ColumnMetadata.named("changeset_id").ofType(-5).withSize(19).notNull());
        addMetadata(id, ColumnMetadata.named("id").ofType(-5).withSize(19).notNull());
        addMetadata(tags, ColumnMetadata.named("tags").ofType(1111).withSize(2147483647));
        addMetadata(timestamp, ColumnMetadata.named("timestamp").ofType(93).withSize(29).withDigits(6).notNull());
        addMetadata(version, ColumnMetadata.named("version").ofType(-5).withSize(19).notNull());
        addMetadata(visible, ColumnMetadata.named("visible").ofType(-7).withSize(1).notNull());
    }

}

