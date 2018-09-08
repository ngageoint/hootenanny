package hoot.services.models.db;

import static com.querydsl.core.types.PathMetadataFactory.forVariable;

import java.sql.Types;

import com.querydsl.core.types.Path;
import com.querydsl.core.types.PathMetadata;
import com.querydsl.core.types.dsl.NumberPath;
import com.querydsl.core.types.dsl.StringPath;
import com.querydsl.sql.ColumnMetadata;

public class QUserSessions extends com.querydsl.sql.RelationalPathBase<UserSessions> {
    private static final long serialVersionUID = 1946787298195725756L;
    public static final QUserSessions usersessions = new QUserSessions("user_sessions");
    public final StringPath session_id = createString("session_id");
    public final NumberPath<Long> user_id = createNumber("user_id", Long.class);

    public QUserSessions(String variable) {
        super(UserSessions.class, forVariable(variable), "public", "user_sessions");
        addMetadata();
    }

    public QUserSessions(String variable, String schema, String table) {
        super(UserSessions.class, forVariable(variable), schema, table);
        addMetadata();
    }

    public QUserSessions(Path<? extends UserSessions> path) {
        super(path.getType(), path.getMetadata(), "public", "user_sessions");
        addMetadata();
    }

    public QUserSessions(PathMetadata metadata) {
        super(UserSessions.class, metadata, "public", "user_sessions");
        addMetadata();
    }

    public void addMetadata() {
        addMetadata(session_id, ColumnMetadata.named("session_id").withIndex(1).ofType(Types.CHAR).withSize(36).notNull());
        addMetadata(user_id, ColumnMetadata.named("user_id").withIndex(2).ofType(Types.BIGINT).withSize(19).notNull());
    }
}
