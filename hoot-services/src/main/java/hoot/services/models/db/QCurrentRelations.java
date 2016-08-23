package hoot.services.models.db;

import static com.querydsl.core.types.PathMetadataFactory.*;

import com.querydsl.core.types.dsl.*;

import com.querydsl.core.types.PathMetadata;
import javax.annotation.Generated;
import com.querydsl.core.types.Path;

import com.querydsl.sql.ColumnMetadata;
import java.sql.Types;




/**
 * QCurrentRelations is a Querydsl query type for CurrentRelations
 */
@Generated("com.querydsl.sql.codegen.MetaDataSerializer")
public class QCurrentRelations extends com.querydsl.sql.RelationalPathBase<CurrentRelations> {

    private static final long serialVersionUID = 1195103832;

    public static final QCurrentRelations currentRelations = new QCurrentRelations("current_relations");

    public final NumberPath<Long> changesetId = createNumber("changesetId", Long.class);

    public final NumberPath<Long> id = createNumber("id", Long.class);

    public final SimplePath<Object> tags = createSimple("tags", Object.class);

    public final DateTimePath<java.sql.Timestamp> timestamp = createDateTime("timestamp", java.sql.Timestamp.class);

    public final NumberPath<Long> version = createNumber("version", Long.class);

    public final BooleanPath visible = createBoolean("visible");

    public final com.querydsl.sql.PrimaryKey<CurrentRelations> currentRelationsPkey = createPrimaryKey(id);

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

    public QCurrentRelations(PathMetadata metadata) {
        super(CurrentRelations.class, metadata, "public", "current_relations");
        addMetadata();
    }

    public void addMetadata() {
        addMetadata(changesetId, ColumnMetadata.named("changeset_id").withIndex(2).ofType(Types.BIGINT).withSize(19).notNull());
        addMetadata(id, ColumnMetadata.named("id").withIndex(1).ofType(Types.BIGINT).withSize(19).notNull());
        addMetadata(tags, ColumnMetadata.named("tags").withIndex(6).ofType(Types.OTHER).withSize(2147483647));
        addMetadata(timestamp, ColumnMetadata.named("timestamp").withIndex(3).ofType(Types.TIMESTAMP).withSize(29).withDigits(6).notNull());
        addMetadata(version, ColumnMetadata.named("version").withIndex(5).ofType(Types.BIGINT).withSize(19).notNull());
        addMetadata(visible, ColumnMetadata.named("visible").withIndex(4).ofType(Types.BIT).withSize(1).notNull());
    }

}

