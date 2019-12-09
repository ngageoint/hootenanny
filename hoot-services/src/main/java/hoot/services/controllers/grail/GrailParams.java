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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.grail;

import java.io.File;

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.annotation.JsonProperty;

import hoot.services.models.db.Users;


/**
 * Used as a more uniform object across all the grail endpoints for when json data is being
 * consumed by the endpoint but also as a more simple way of transferring data to other classes
 * with 1 single object instead of multiple parameters
 */
@JsonIgnoreProperties(ignoreUnknown = true)
public class GrailParams {

    @JsonProperty("BBOX")
    private String bounds;

    @JsonProperty("APPLY_TAGS")
    private Boolean applyTags = false;

    @JsonIgnore
    private Users user;

    @JsonProperty("input1")
    private String input1;

    @JsonProperty("input2")
    private String input2;

    @JsonProperty("output")
    private String output;

    @JsonProperty("parentId")
    private String parentId;

    @JsonProperty("pullUrl")
    private String pullUrl;

    @JsonProperty("pushUrl")
    private String pushUrl;

    @JsonProperty("maxBBoxSize")
    private Double maxSize;

    @JsonProperty("customQuery")
    private String customQuery;

    private String consumerKey;

    private String consumerSecret;

    private File workDir;

    private String conflationType;

    private String comment;

    private String hashtags;

    private String source;

    public GrailParams() {}

    public GrailParams(GrailParams params) {
        this.bounds = params.getBounds();
        this.applyTags = params.getApplyTags();
        this.user = params.getUser();
        this.input1 = params.getInput1();
        this.input2 = params.getInput2();
        this.output = params.getOutput();
        this.parentId = params.getParentId();
        this.pullUrl = params.getPullUrl();
        this.pushUrl = params.getPushUrl();
        this.maxSize = params.getMaxBBoxSize();
        this.customQuery = params.getCustomQuery();
        this.consumerKey = params.getConsumerKey();
        this.consumerSecret = params.getConsumerSecret();
        this.workDir = params.getWorkDir();
        this.conflationType = params.getConflationType();
        this.comment = params.getComment();
    }

    public String getComment() {
        return comment;
    }

    public void setComment(String comment) {
        this.comment = comment;
    }

    public String getHashtags() {
        return hashtags;
    }

    public void setHashtags(String hashtags) {
        this.hashtags = hashtags;
    }

    public String getSource() {
        return source;
    }

    public void setSource(String source) {
        this.source = source;
    }

    public String getConflationType() {
        return conflationType;
    }

    public void setConflationType(String conflationType) {
        this.conflationType = conflationType;
    }

    public File getWorkDir() {
        return workDir;
    }

    public void setWorkDir(File workDir) {
        this.workDir = workDir;
    }

    public String getBounds() {
        return bounds;
    }

    public void setBounds(String bounds) {
        this.bounds = bounds;
    }

    public Boolean getApplyTags() {
        return applyTags;
    }

    public void setApplyTags(Boolean applyTags) {
        this.applyTags = applyTags;
    }

    public Users getUser() {
        return user;
    }

    public void setUser(Users user) {
        this.user = user;
    }

    public String getInput1() {
        return input1;
    }

    public void setInput1(String input1) {
        this.input1 = input1;
    }

    public String getInput2() {
        return input2;
    }

    public void setInput2(String input2) {
        this.input2 = input2;
    }

    public String getOutput() {
        return output;
    }

    public void setOutput(String output) {
        this.output = output;
    }

    public String getParentId() {
        return parentId;
    }

    public void setParentId(String parentId) {
        this.parentId = parentId;
    }

    public String getPushUrl() {
        return pushUrl;
    }

    public void setPushUrl(String pushUrl) {
        this.pushUrl = pushUrl;
    }

    public String getPullUrl() {
        return pullUrl;
    }

    public void setPullUrl(String pullUrl) {
        this.pullUrl = pullUrl;
    }

    public Double getMaxBBoxSize() {
        return (this.maxSize == null) ? 0.25 : maxSize;
    }

    public void setMaxBBoxSize(Double maxSize) {
        this.maxSize = maxSize;
    }

    public void setConsumerKey(String key) {
        consumerKey = key;
    }

    public String getConsumerKey() {
        return consumerKey;
    }

    public void setConsumerSecret(String secret) {
        consumerSecret = secret;
    }

    public String getConsumerSecret() {
        return consumerSecret;
    }

    public void setCustomQuery(String customQuery) {
        this.customQuery = customQuery;
    }

    public String getCustomQuery() {
        return customQuery;
    }

    @Override
    public String toString() {
        return "GrailParams{" +
                "BBOX='" + bounds + '\'' +
                ", USER_ID='" + user.getDisplayName() + '\'' +
                ", input1='" + input1 + '\'' +
                ", input2='" + input2 + '\'' +
                ", output='" + output + '\'' +
                ", parentId='" + parentId + '\'' +
                ", pushUrl='" + pushUrl + '\'' +
                ", pullUrl='" + pullUrl + '\'' +
                ", maxBBoxSize='" + maxSize + '\'' +
                '}';
    }
}
