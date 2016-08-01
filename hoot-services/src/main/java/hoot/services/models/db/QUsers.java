package hoot.services.models.db;

import static com.mysema.query.types.PathMetadataFactory.forVariable;

import javax.annotation.Generated;

import com.mysema.query.sql.ColumnMetadata;
import com.mysema.query.types.Path;
import com.mysema.query.types.PathMetadata;
import com.mysema.query.types.path.NumberPath;
import com.mysema.query.types.path.StringPath;




/**
 * QUsers is a Querydsl query type for Users
 */
@Generated("com.mysema.query.sql.codegen.MetaDataSerializer")
public class QUsers extends com.mysema.query.sql.RelationalPathBase<Users> {

    private static final long serialVersionUID = 1881840750;

    public static final QUsers users = new QUsers("users");

    public final StringPath displayName = createString("displayName");

    public final StringPath email = createString("email");

    public final NumberPath<Long> id = createNumber("id", Long.class);

    public final com.mysema.query.sql.PrimaryKey<Users> usersPk = createPrimaryKey(id);

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

    public QUsers(PathMetadata<?> metadata) {
        super(Users.class, metadata, "public", "users");
        addMetadata();
    }

    public void addMetadata() {
        addMetadata(displayName, ColumnMetadata.named("display_name").ofType(12).withSize(255).notNull());
        addMetadata(email, ColumnMetadata.named("email").ofType(12).withSize(255).notNull());
        addMetadata(id, ColumnMetadata.named("id").ofType(-5).withSize(19).notNull());
    }

}

