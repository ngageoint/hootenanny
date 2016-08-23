package hoot.services.models.db;

import static com.querydsl.core.types.PathMetadataFactory.*;

import com.querydsl.core.types.dsl.*;

import com.querydsl.core.types.PathMetadata;
import javax.annotation.Generated;
import com.querydsl.core.types.Path;

import com.querydsl.sql.ColumnMetadata;
import java.sql.Types;




/**
 * QDatabasechangeloglock is a Querydsl query type for Databasechangeloglock
 */
@Generated("com.querydsl.sql.codegen.MetaDataSerializer")
public class QDatabasechangeloglock extends com.querydsl.sql.RelationalPathBase<Databasechangeloglock> {

    private static final long serialVersionUID = -166298230;

    public static final QDatabasechangeloglock databasechangeloglock = new QDatabasechangeloglock("databasechangeloglock");

    public final NumberPath<Integer> id = createNumber("id", Integer.class);

    public final BooleanPath locked = createBoolean("locked");

    public final StringPath lockedby = createString("lockedby");

    public final DateTimePath<java.sql.Timestamp> lockgranted = createDateTime("lockgranted", java.sql.Timestamp.class);

    public final com.querydsl.sql.PrimaryKey<Databasechangeloglock> databasechangeloglockPk = createPrimaryKey(id);

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

    public QDatabasechangeloglock(PathMetadata metadata) {
        super(Databasechangeloglock.class, metadata, "public", "databasechangeloglock");
        addMetadata();
    }

    public void addMetadata() {
        addMetadata(id, ColumnMetadata.named("id").withIndex(1).ofType(Types.INTEGER).withSize(10).notNull());
        addMetadata(locked, ColumnMetadata.named("locked").withIndex(2).ofType(Types.BIT).withSize(1).notNull());
        addMetadata(lockedby, ColumnMetadata.named("lockedby").withIndex(4).ofType(Types.VARCHAR).withSize(255));
        addMetadata(lockgranted, ColumnMetadata.named("lockgranted").withIndex(3).ofType(Types.TIMESTAMP).withSize(35).withDigits(6));
    }

}

