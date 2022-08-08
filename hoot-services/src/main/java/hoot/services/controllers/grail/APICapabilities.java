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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.grail;

/**
 * Used to store information for a getCapabilities request results
 */
public class APICapabilities {
    private Double minVersion;
    private Double maxVersion;
    private Double maxArea;
    private Long waynodes;
    private Long changesets;
    private Long timeout;
    private String databaseStatus;
    private String apiStatus;
    private String gpxStatus;


    public Double getMinVersion() {
        return this.minVersion;
    }

    public void setMinVersion(Double minVersion) {
        this.minVersion = minVersion;
    }

    public Double getMaxVersion() {
        return this.maxVersion;
    }

    public void setMaxVersion(Double maxVersion) {
        this.maxVersion = maxVersion;
    }

    public Double getMaxArea() {
        return this.maxArea;
    }

    public void setMaxArea(Double maxArea) {
        this.maxArea = maxArea;
    }

    public Long getWaynodes() {
        return this.waynodes;
    }

    public void setWaynodes(Long waynodes) {
        this.waynodes = waynodes;
    }

    public Long getChangesets() {
        return this.changesets;
    }

    public void setChangesets(Long changesets) {
        this.changesets = changesets;
    }

    public Long getTimeout() {
        return this.timeout;
    }

    public void setTimeout(Long timeout) {
        this.timeout = timeout;
    }

    public String getDatabaseStatus() {
        return this.databaseStatus;
    }

    public void setDatabaseStatus(String databaseStatus) {
        this.databaseStatus = databaseStatus;
    }

    public String getApiStatus() {
        return this.apiStatus;
    }

    public void setApiStatus(String apiStatus) {
        this.apiStatus = apiStatus;
    }

    public String getGpxStatus() {
        return this.gpxStatus;
    }

    public void setGpxStatus(String gpxStatus) {
        this.gpxStatus = gpxStatus;
    }


    @Override
    public String toString() {
        return "APIParams{" +
                "minVersion='" + minVersion + '\'' +
                ", maxVersion='" + maxVersion + '\'' +
                ", maxArea='" + maxArea + '\'' +
                ", waynodes='" + waynodes + '\'' +
                ", changesets='" + changesets + '\'' +
                ", timeout='" + timeout + '\'' +
                ", databaseStatus='" + databaseStatus + '\'' +
                ", apiStatus='" + apiStatus + '\'' +
                ", gpxStatus='" + gpxStatus + '\'' +
                '}';
    }

}
