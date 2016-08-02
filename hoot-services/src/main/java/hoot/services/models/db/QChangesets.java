package hoot.services.models.db;

import static com.mysema.query.types.PathMetadataFactory.*;

import com.mysema.query.types.path.*;

import com.mysema.query.types.PathMetadata;
import javax.annotation.Generated;
import com.mysema.query.types.Path;

import com.mysema.query.sql.ColumnMetadata;




/**
 * QChangesets is a Querydsl query type for Changesets
 */
@Generated("com.mysema.query.sql.codegen.MetaDataSerializer")
public class QChangesets extends com.mysema.query.sql.RelationalPathBase<Changesets> {

    private static final long serialVersionUID = -69300901;

    public static final QChangesets changesets = new QChangesets("changesets");

    public final DateTimePath<java.sql.Timestamp> closedAt = createDateTime("closedAt", java.sql.Timestamp.class);

    public final DateTimePath<java.sql.Timestamp> createdAt = createDateTime("createdAt", java.sql.Timestamp.class);

    public final NumberPath<Long> id = createNumber("id", Long.class);

    public final NumberPath<Double> maxLat = createNumber("maxLat", Double.class);

    public final NumberPath<Double> maxLon = createNumber("maxLon", Double.class);

    public final NumberPath<Double> minLat = createNumber("minLat", Double.class);

    public final NumberPath<Double> minLon = createNumber("minLon", Double.class);

    public final NumberPath<Integer> numChanges = createNumber("numChanges", Integer.class);

    public final SimplePath<Object> tags = createSimple("tags", Object.class);

    public final NumberPath<Long> userId = createNumber("userId", Long.class);

    public final com.mysema.query.sql.PrimaryKey<Changesets> changesetsPk = createPrimaryKey(id);

    public QChangesets(String variable) {
        super(Changesets.class, forVariable(variable), "public", "changesets");
        addMetadata();
    }

    public QChangesets(String variable, String schema, String table) {
        super(Changesets.class, forVariable(variable), schema, table);
        addMetadata();
    }

    public QChangesets(Path<? extends Changesets> path) {
        super(path.getType(), path.getMetadata(), "public", "changesets");
        addMetadata();
    }

    public QChangesets(PathMetadata<?> metadata) {
        super(Changesets.class, metadata, "public", "changesets");
        addMetadata();
    }

    public void addMetadata() {
        addMetadata(closedAt, ColumnMetadata.named("closed_at").ofType(93).withSize(29).withDigits(6).notNull());
        addMetadata(createdAt, ColumnMetadata.named("created_at").ofType(93).withSize(29).withDigits(6).notNull());
        addMetadata(id, ColumnMetadata.named("id").ofType(-5).withSize(19).notNull());
        addMetadata(maxLat, ColumnMetadata.named("max_lat").ofType(8).withSize(17).withDigits(17).notNull());
        addMetadata(maxLon, ColumnMetadata.named("max_lon").ofType(8).withSize(17).withDigits(17).notNull());
        addMetadata(minLat, ColumnMetadata.named("min_lat").ofType(8).withSize(17).withDigits(17).notNull());
        addMetadata(minLon, ColumnMetadata.named("min_lon").ofType(8).withSize(17).withDigits(17).notNull());
        addMetadata(numChanges, ColumnMetadata.named("num_changes").ofType(4).withSize(10).notNull());
        addMetadata(tags, ColumnMetadata.named("tags").ofType(1111).withSize(2147483647));
        addMetadata(userId, ColumnMetadata.named("user_id").ofType(-5).withSize(19).notNull());
    }

}

