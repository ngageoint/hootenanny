package hoot.services.models.db;

import static com.querydsl.core.types.PathMetadataFactory.*;

import com.querydsl.core.types.dsl.*;

import com.querydsl.core.types.PathMetadata;
import javax.annotation.Generated;
import com.querydsl.core.types.Path;

import com.querydsl.sql.ColumnMetadata;
import java.sql.Types;




/**
 * QUsers is a Querydsl query type for Users
 */
@Generated("com.querydsl.sql.codegen.MetaDataSerializer")
public class QUsers extends com.querydsl.sql.RelationalPathBase<Users> {

    private static final long serialVersionUID = 1881840750;

    public static final QUsers users = new QUsers("users");

    public final StringPath displayName = createString("displayName");

    public final StringPath email = createString("email");

    public final NumberPath<Long> id = createNumber("id", Long.class);

    public final com.querydsl.sql.PrimaryKey<Users> usersPk = createPrimaryKey(id);

    public QUsers(String variable) {
        super(Users.class, forVariable(variable), "public", "users");
        addMetadata();
    }

    public QUsers(String variable, String schema, String table) {
        super(Users.class, forVariable(variable), schema, table);
        addMetadata();
    }

    public QUsers(Path<? extends Users> path) {
        super(path.getType(), path.getMetadata(), "public", "users");
        addMetadata();
    }

    public QUsers(PathMetadata metadata) {
        super(Users.class, metadata, "public", "users");
        addMetadata();
    }

    public void addMetadata() {
        addMetadata(displayName, ColumnMetadata.named("display_name").withIndex(3).ofType(Types.VARCHAR).withSize(255).notNull());
        addMetadata(email, ColumnMetadata.named("email").withIndex(1).ofType(Types.VARCHAR).withSize(255).notNull());
        addMetadata(id, ColumnMetadata.named("id").withIndex(2).ofType(Types.BIGINT).withSize(19).notNull());
    }

}

