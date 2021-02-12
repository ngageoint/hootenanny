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

import javax.annotation.Generated;

/**
 * JobStatus is a Querydsl bean type
 */
@Generated("com.querydsl.codegen.BeanSerializer")
public class JobStatus {

    private java.sql.Timestamp end;

    private String jobId;

    private Integer percentComplete;

    private java.sql.Timestamp start;

    private Integer status;

    private String statusDetail;

    private Long resourceId;

    private Long userId;

    private Integer jobType;

    private Integer trackableCommandCount;

    private Object tags;

    public java.sql.Timestamp getEnd() {
        return end;
    }

    public void setEnd(java.sql.Timestamp end) {
        this.end = end;
    }

    public String getJobId() {
        return jobId;
    }

    public void setJobId(String jobId) {
        this.jobId = jobId;
    }

    public Integer getPercentComplete() {
        return percentComplete;
    }

    public void setPercentComplete(Integer percentComplete) {
        this.percentComplete = percentComplete;
    }

    public java.sql.Timestamp getStart() {
        return start;
    }

    public void setStart(java.sql.Timestamp start) {
        this.start = start;
    }

    public Integer getStatus() {
        return status;
    }

    public void setStatus(Integer status) {
        this.status = status;
    }

    public String getStatusDetail() {
        return statusDetail;
    }

    public void setStatusDetail(String statusDetail) {
        this.statusDetail = statusDetail;
    }

    public Long getResourceId() {
        return resourceId;
    }

    public void setResourceId(Long resourceId) {
        this.resourceId = resourceId;
    }

    public Long getUserId() {
        return userId;
    }

    public void setUserId(Long userId) {
        this.userId = userId;
    }

    public Integer getJobType() {
        return jobType;
    }

    public void setJobType(Integer jobType) {
        this.jobType = jobType;
    }

    public Integer getCommandCount() {
        return trackableCommandCount;
    }

    public void setCommandCount(Integer commandCount) {
        this.trackableCommandCount = commandCount;
    }

    public Object getTags() {
        return tags;
    }

    public void setTags(Object tags) {
        this.tags = tags;
    }
}

