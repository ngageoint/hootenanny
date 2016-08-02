package hoot.services.models.db;

import static com.mysema.query.types.PathMetadataFactory.*;

import com.mysema.query.types.path.*;

import com.mysema.query.types.PathMetadata;
import javax.annotation.Generated;
import com.mysema.query.types.Path;

import com.mysema.query.sql.ColumnMetadata;




/**
 * QDatabasechangelog is a Querydsl query type for Databasechangelog
 */
@Generated("com.mysema.query.sql.codegen.MetaDataSerializer")
public class QDatabasechangelog extends com.mysema.query.sql.RelationalPathBase<Databasechangelog> {

    private static final long serialVersionUID = -1877688353;

    public static final QDatabasechangelog databasechangelog = new QDatabasechangelog("databasechangelog");

    public final StringPath author = createString("author");

    public final StringPath comments = createString("comments");

    public final DateTimePath<java.sql.Timestamp> dateexecuted = createDateTime("dateexecuted", java.sql.Timestamp.class);

    public final StringPath description = createString("description");

    public final StringPath exectype = createString("exectype");

    public final StringPath filename = createString("filename");

    public final StringPath id = createString("id");

    public final StringPath liquibase = createString("liquibase");

    public final StringPath md5sum = createString("md5sum");

    public final NumberPath<Integer> orderexecuted = createNumber("orderexecuted", Integer.class);

    public final StringPath tag = createString("tag");

    public QDatabasechangelog(String variable) {
        super(Databasechangelog.class, forVariable(variable), "public", "databasechangelog");
        addMetadata();
    }

    public QDatabasechangelog(String variable, String schema, String table) {
        super(Databasechangelog.class, forVariable(variable), schema, table);
        addMetadata();
    }

    public QDatabasechangelog(Path<? extends Databasechangelog> path) {
        super(path.getType(), path.getMetadata(), "public", "databasechangelog");
        addMetadata();
    }

    public QDatabasechangelog(PathMetadata<?> metadata) {
        super(Databasechangelog.class, metadata, "public", "databasechangelog");
        addMetadata();
    }

    public void addMetadata() {
        addMetadata(author, ColumnMetadata.named("author").ofType(12).withSize(255).notNull());
        addMetadata(comments, ColumnMetadata.named("comments").ofType(12).withSize(255));
        addMetadata(dateexecuted, ColumnMetadata.named("dateexecuted").ofType(93).withSize(35).withDigits(6).notNull());
        addMetadata(description, ColumnMetadata.named("description").ofType(12).withSize(255));
        addMetadata(exectype, ColumnMetadata.named("exectype").ofType(12).withSize(10).notNull());
        addMetadata(filename, ColumnMetadata.named("filename").ofType(12).withSize(255).notNull());
        addMetadata(id, ColumnMetadata.named("id").ofType(12).withSize(255).notNull());
        addMetadata(liquibase, ColumnMetadata.named("liquibase").ofType(12).withSize(20));
        addMetadata(md5sum, ColumnMetadata.named("md5sum").ofType(12).withSize(35));
        addMetadata(orderexecuted, ColumnMetadata.named("orderexecuted").ofType(4).withSize(10).notNull());
        addMetadata(tag, ColumnMetadata.named("tag").ofType(12).withSize(255));
    }

}

