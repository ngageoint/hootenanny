package hoot.services.models.db;

import static com.mysema.query.types.PathMetadataFactory.*;

import com.mysema.query.types.path.*;

import com.mysema.query.types.PathMetadata;
import javax.annotation.Generated;
import com.mysema.query.types.Path;

import com.mysema.query.sql.ColumnMetadata;




/**
 * QCurrentRelations is a Querydsl query type for CurrentRelations
 */
@Generated("com.mysema.query.sql.codegen.MetaDataSerializer")
public class QCurrentRelations extends com.mysema.query.sql.RelationalPathBase<CurrentRelations> {

    private static final long serialVersionUID = 1195103832;

    public static final QCurrentRelations currentRelations = new QCurrentRelations("current_relations");

    public final NumberPath<Long> changesetId = createNumber("changesetId", Long.class);

    public final NumberPath<Long> id = createNumber("id", Long.class);

    public final SimplePath<Object> tags = createSimple("tags", Object.class);

    public final DateTimePath<java.sql.Timestamp> timestamp = createDateTime("timestamp", java.sql.Timestamp.class);

    public final NumberPath<Long> version = createNumber("version", Long.class);

    public final BooleanPath visible = createBoolean("visible");

    public final com.mysema.query.sql.PrimaryKey<CurrentRelations> currentRelationsPkey = createPrimaryKey(id);

    public QCurrentRelations(String variable) {
        super(CurrentRelations.class, forVariable(variable), "public", "current_relations");
        addMetadata();
    }

    public QCurrentRelations(String variable, String schema, String table) {
        super(CurrentRelations.class, forVariable(variable), schema, table);
        addMetadata();
    }

    public QCurrentRelations(Path<? extends CurrentRelations> path) {
        super(path.getType(), path.getMetadata(), "public", "current_relations");
        addMetadata();
    }

    public QCurrentRelations(PathMetadata<?> metadata) {
        super(CurrentRelations.class, metadata, "public", "current_relations");
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

