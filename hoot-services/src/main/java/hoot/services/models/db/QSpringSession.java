package hoot.services.models.db;

import static com.querydsl.core.types.PathMetadataFactory.forVariable;

import java.sql.Types;

import com.querydsl.core.types.Path;
import com.querydsl.core.types.PathMetadata;
import com.querydsl.core.types.dsl.NumberPath;
import com.querydsl.core.types.dsl.StringPath;
import com.querydsl.sql.ColumnMetadata;

public class QSpringSession extends com.querydsl.sql.RelationalPathBase<SpringSession> {
    private static final long serialVersionUID = 4196481175646408888L;
    public static final QSpringSession springsessions = new QSpringSession("spring_session");

    public final StringPath session_id = createString("session_id");
    public final NumberPath<Long> creation_time = createNumber("creation_time", Long.class);
    public final NumberPath<Long> last_access_time = createNumber("last_access_time", Long.class);
    public final NumberPath<Integer> max_inactive_interval = createNumber("max_inactive_interval", Integer.class);
    public final StringPath principal_name = createString("principal_name");

    public QSpringSession(String variable) {
        super(SpringSession.class, forVariable(variable), "public", "spring_session");
        addMetadata();
    }

    public QSpringSession(String variable, String schema, String table) {
        super(SpringSession.class, forVariable(variable), schema, table);
        addMetadata();
    }

    public QSpringSession(Path<? extends SpringSession> path) {
        super(path.getType(), path.getMetadata(), "public", "spring_session");
        addMetadata();
    }

    public QSpringSession(PathMetadata metadata) {
        super(SpringSession.class, metadata, "public", "spring_session");
        addMetadata();
    }

    public void addMetadata() {
        addMetadata(session_id, ColumnMetadata.named("session_id").withIndex(1).ofType(Types.CHAR).withSize(36).notNull());
        addMetadata(creation_time, ColumnMetadata.named("creation_time").withIndex(2).ofType(Types.BIGINT).withSize(19).notNull());
        addMetadata(last_access_time, ColumnMetadata.named("last_access_time").withIndex(3).ofType(Types.BIGINT).withSize(19).notNull());
        addMetadata(max_inactive_interval, ColumnMetadata.named("max_inactive_interval").withIndex(4).ofType(Types.INTEGER).withSize(10).notNull());
        addMetadata(principal_name, ColumnMetadata.named("principal_name").withIndex(5).ofType(Types.VARCHAR).withSize(100).notNull());
    }

}
