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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.grail;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.annotation.JsonProperty;


@JsonIgnoreProperties(ignoreUnknown = true)
public class GrailParams {

    @JsonProperty("BBOX")
    private String bounds;

    @JsonProperty("USER_ID")
    private String userId;

    @JsonProperty("input1")
    private String input1;

    @JsonProperty("input2")
    private String input2;

    @JsonProperty("output")
    private String output;

    @JsonProperty("pullUrl")
    private String pullUrl;

    @JsonProperty("pushUrl")
    private String pushUrl;

    @JsonProperty("maxBBoxSize")
    private Double maxSize;

    @JsonProperty("capabilitiesUrl")
    private String capabilitiesUrl;


    public String getBounds() {
        return bounds;
    }

    public void setBounds(String bounds) {
        this.bounds = bounds;
    }

    public String getUserId() {
        return (this.userId == null) ? "Hootenanny" : userId;
    }

    public void setUserId(String userId) {
        this.userId = userId;
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

    public String getCapabilitiesUrl() {
        return capabilitiesUrl;
    }

    public void setCapabilitiesUrl(String capabilitiesUrl) {
        this.capabilitiesUrl = capabilitiesUrl;
    }


    @Override
    public String toString() {
        return "ExportParams{" +
                "BBOX='" + bounds + '\'' +
                ", USER_ID='" + userId + '\'' +
                ", input1='" + input1 + '\'' +
                ", input2='" + input2 + '\'' +
                ", output='" + output + '\'' +
                ", pushUrl='" + pushUrl + '\'' +
                ", pullUrl='" + pullUrl + '\'' +
                ", capabilitiesUrl='" + capabilitiesUrl + '\'' +
                ", maxBBoxSize='" + maxSize + '\'' +
                '}';
    }
}
