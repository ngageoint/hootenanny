package hoot.services.models.db;

import static com.mysema.query.types.PathMetadataFactory.*;

import com.mysema.query.types.path.*;

import com.mysema.query.types.PathMetadata;
import javax.annotation.Generated;
import com.mysema.query.types.Path;

import com.mysema.query.sql.ColumnMetadata;




/**
 * QDatabasechangeloglock is a Querydsl query type for Databasechangeloglock
 */
@Generated("com.mysema.query.sql.codegen.MetaDataSerializer")
public class QDatabasechangeloglock extends com.mysema.query.sql.RelationalPathBase<Databasechangeloglock> {

    private static final long serialVersionUID = -166298230;

    public static final QDatabasechangeloglock databasechangeloglock = new QDatabasechangeloglock("databasechangeloglock");

    public final NumberPath<Integer> id = createNumber("id", Integer.class);

    public final BooleanPath locked = createBoolean("locked");

    public final StringPath lockedby = createString("lockedby");

    public final DateTimePath<java.sql.Timestamp> lockgranted = createDateTime("lockgranted", java.sql.Timestamp.class);

    public final com.mysema.query.sql.PrimaryKey<Databasechangeloglock> databasechangeloglockPk = createPrimaryKey(id);

    public QDatabasechangeloglock(String variable) {
        super(Databasechangeloglock.class, forVariable(variable), "public", "databasechangeloglock");
        addMetadata();
    }

    public QDatabasechangeloglock(String variable, String schema, String table) {
        super(Databasechangeloglock.class, forVariable(variable), schema, table);
        addMetadata();
    }

    public QDatabasechangeloglock(Path<? extends Databasechangeloglock> path) {
        super(path.getType(), path.getMetadata(), "public", "databasechangeloglock");
        addMetadata();
    }

    public QDatabasechangeloglock(PathMetadata<?> metadata) {
        super(Databasechangeloglock.class, metadata, "public", "databasechangeloglock");
        addMetadata();
    }

    public void addMetadata() {
        addMetadata(id, ColumnMetadata.named("id").ofType(4).withSize(10).notNull());
        addMetadata(locked, ColumnMetadata.named("locked").ofType(-7).withSize(1).notNull());
        addMetadata(lockedby, ColumnMetadata.named("lockedby").ofType(12).withSize(255));
        addMetadata(lockgranted, ColumnMetadata.named("lockgranted").ofType(93).withSize(35).withDigits(6));
    }

}

