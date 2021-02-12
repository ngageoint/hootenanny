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
 * @copyright Copyright (C) 2016, 2018, 2019 Maxar (http://www.maxar.com/)
 */
package hoot.services.models.db;

import static com.querydsl.core.types.PathMetadataFactory.forVariable;

import java.sql.Types;

import javax.annotation.Generated;

import com.querydsl.core.types.Path;
import com.querydsl.core.types.PathMetadata;
import com.querydsl.core.types.dsl.DateTimePath;
import com.querydsl.core.types.dsl.NumberPath;
import com.querydsl.core.types.dsl.SimplePath;
import com.querydsl.core.types.dsl.StringPath;
import com.querydsl.sql.ColumnMetadata;




/**
 * QJobStatus is a Querydsl query type for JobStatus
 */
@Generated("com.querydsl.sql.codegen.MetaDataSerializer")
public class QJobStatus extends com.querydsl.sql.RelationalPathBase<JobStatus> {

    private static final long serialVersionUID = 364396917;

    public static final QJobStatus jobStatus = new QJobStatus("job_status");

    public final StringPath jobId = createString("jobId");

    public final com.querydsl.sql.PrimaryKey<JobStatus> jobStatusPkey = createPrimaryKey(jobId);

    public final DateTimePath<java.sql.Timestamp> start = createDateTime("start", java.sql.Timestamp.class);

    public final DateTimePath<java.sql.Timestamp> end = createDateTime("end", java.sql.Timestamp.class);

    public final NumberPath<Integer> status = createNumber("status", Integer.class);

    public final NumberPath<Integer> percentComplete = createNumber("percentComplete", Integer.class);

    public final StringPath statusDetail = createString("statusDetail");

    public final NumberPath<Long> resourceId = createNumber("resourceId", Long.class);

    public final NumberPath<Long> userId = createNumber("userId", Long.class);

    public final NumberPath<Integer> jobType = createNumber("jobType", Integer.class);

    public final NumberPath<Integer> trackableCommandCount = createNumber("trackableCommandCount", Integer.class);

    public final SimplePath<Object> tags = createSimple("tags", Object.class);

    public QJobStatus(String variable) {
        super(JobStatus.class, forVariable(variable), "public", "job_status");
        addMetadata();
    }

    public QJobStatus(String variable, String schema, String table) {
        super(JobStatus.class, forVariable(variable), schema, table);
        addMetadata();
    }

    public QJobStatus(Path<? extends JobStatus> path) {
        super(path.getType(), path.getMetadata(), "public", "job_status");
        addMetadata();
    }

    public QJobStatus(PathMetadata metadata) {
        super(JobStatus.class, metadata, "public", "job_status");
        addMetadata();
    }

    public void addMetadata() {
        addMetadata(jobId, ColumnMetadata.named("job_id").withIndex(1).ofType(Types.VARCHAR).withSize(64).notNull());
        addMetadata(start, ColumnMetadata.named("start").withIndex(2).ofType(Types.TIMESTAMP).withSize(35).withDigits(6).notNull());
        addMetadata(end, ColumnMetadata.named("end").withIndex(3).ofType(Types.TIMESTAMP).withSize(35).withDigits(6));
        addMetadata(status, ColumnMetadata.named("status").withIndex(4).ofType(Types.INTEGER).withSize(10).notNull());
        addMetadata(percentComplete, ColumnMetadata.named("percent_complete").withIndex(5).ofType(Types.INTEGER).withSize(10));
        addMetadata(statusDetail, ColumnMetadata.named("status_detail").withIndex(6).ofType(Types.VARCHAR).withSize(2147483647));
        addMetadata(resourceId, ColumnMetadata.named("resource_id").withIndex(7).ofType(Types.BIGINT).withSize(19).notNull());
        addMetadata(userId, ColumnMetadata.named("user_id").withIndex(8).ofType(Types.BIGINT).withSize(19).notNull());
        addMetadata(jobType, ColumnMetadata.named("job_type").withIndex(9).ofType(Types.INTEGER).withSize(10));
        addMetadata(trackableCommandCount, ColumnMetadata.named("trackable_command_count").withIndex(10).ofType(Types.INTEGER).withSize(10));
        addMetadata(tags, ColumnMetadata.named("tags").withIndex(11).ofType(Types.OTHER).withSize(2147483647));
    }

}

