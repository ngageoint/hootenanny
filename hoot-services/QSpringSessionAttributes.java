package hoot.services.models.db;

import static com.querydsl.core.types.PathMetadataFactory.forVariable;

import java.sql.Types;

import com.querydsl.core.types.Path;
import com.querydsl.core.types.PathMetadata;
import com.querydsl.core.types.dsl.StringPath;
import com.querydsl.sql.ColumnMetadata;

public class QSpringSessionAttributes extends com.querydsl.sql.RelationalPathBase<SpringSessionAttributes> {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = -604675933836125216L;
	public final StringPath session_id = createString("session_id");
	public final StringPath attribute_name = createString("principal_name");
	
	public QSpringSessionAttributes(String variable) {
        super(SpringSessionAttributes.class, forVariable(variable), "public", "spring_session_attributes");
        addMetadata();
    }

    public QSpringSessionAttributes(String variable, String schema, String table) {
        super(SpringSessionAttributes.class, forVariable(variable), schema, table);
        addMetadata();
    }

    public QSpringSessionAttributes(Path<? extends SpringSessionAttributes> path) {
        super(path.getType(), path.getMetadata(), "public", "spring_session_attributes");
        addMetadata();
    }

    public QSpringSessionAttributes(PathMetadata metadata) {
        super(SpringSessionAttributes.class, metadata, "public", "spring_session_attributes");
        addMetadata();
    }

    public void addMetadata() {
        addMetadata(session_id, ColumnMetadata.named("session_id").withIndex(1).ofType(Types.CHAR).withSize(36).notNull());
        addMetadata(attribute_name, ColumnMetadata.named("attribute_name").withIndex(2).ofType(Types.VARCHAR).withSize(200).notNull());
    }
}
