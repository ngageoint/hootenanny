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
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.models.db;

import static com.querydsl.core.types.PathMetadataFactory.forVariable;

import java.sql.Types;

import javax.annotation.Generated;

import com.querydsl.core.types.Path;
import com.querydsl.core.types.PathMetadata;
import com.querydsl.core.types.dsl.DateTimePath;
import com.querydsl.core.types.dsl.NumberPath;
import com.querydsl.core.types.dsl.StringPath;
import com.querydsl.sql.ColumnMetadata;




/**
 * QCommandStatus is a Querydsl query type for CommandStatus
 */
@Generated("com.querydsl.sql.codegen.MetaDataSerializer")
public class QCommandStatus extends com.querydsl.sql.RelationalPathBase<CommandStatus> {

    private static final long serialVersionUID = -2130509949;

    public static final QCommandStatus commandStatus = new QCommandStatus("command_status");

    public final StringPath command = createString("command");

    public final NumberPath<Integer> exitCode = createNumber("exitCode", Integer.class);

    public final DateTimePath<java.sql.Timestamp> finish = createDateTime("finish", java.sql.Timestamp.class);

    public final NumberPath<Long> id = createNumber("id", Long.class);

    public final StringPath jobId = createString("jobId");

    public final DateTimePath<java.sql.Timestamp> start = createDateTime("start", java.sql.Timestamp.class);

    public final StringPath stderr = createString("stderr");

    public final StringPath stdout = createString("stdout");

    public final com.querydsl.sql.PrimaryKey<CommandStatus> commandStatusPk = createPrimaryKey(id);

    public QCommandStatus(String variable) {
        super(CommandStatus.class, forVariable(variable), "public", "command_status");
        addMetadata();
    }

    public QCommandStatus(String variable, String schema, String table) {
        super(CommandStatus.class, forVariable(variable), schema, table);
        addMetadata();
    }

    public QCommandStatus(Path<? extends CommandStatus> path) {
        super(path.getType(), path.getMetadata(), "public", "command_status");
        addMetadata();
    }

    public QCommandStatus(PathMetadata metadata) {
        super(CommandStatus.class, metadata, "public", "command_status");
        addMetadata();
    }

    public void addMetadata() {
        addMetadata(command, ColumnMetadata.named("command").withIndex(6).ofType(Types.VARCHAR).withSize(2147483647).notNull());
        addMetadata(exitCode, ColumnMetadata.named("exit_code").withIndex(5).ofType(Types.INTEGER).withSize(10));
        addMetadata(finish, ColumnMetadata.named("finish").withIndex(4).ofType(Types.TIMESTAMP).withSize(35).withDigits(6));
        addMetadata(id, ColumnMetadata.named("id").withIndex(1).ofType(Types.BIGINT).withSize(19).notNull());
        addMetadata(jobId, ColumnMetadata.named("job_id").withIndex(2).ofType(Types.VARCHAR).withSize(64));
        addMetadata(start, ColumnMetadata.named("start").withIndex(3).ofType(Types.TIMESTAMP).withSize(35).withDigits(6).notNull());
        addMetadata(stderr, ColumnMetadata.named("stderr").withIndex(8).ofType(Types.VARCHAR).withSize(2147483647).notNull());
        addMetadata(stdout, ColumnMetadata.named("stdout").withIndex(7).ofType(Types.VARCHAR).withSize(2147483647).notNull());
    }

}

