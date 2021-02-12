/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2018 Maxar (http://www.maxar.com/)
 */
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
    public final NumberPath<Long> user_id = createNumber("user_id", Long.class);

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
        addMetadata(user_id, ColumnMetadata.named("user_id").withIndex(6).ofType(Types.BIGINT).withSize(19).notNull());
    }

}
