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
 * @copyright Copyright (C) 2015, 2016, 2017, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.clipping;

import com.fasterxml.jackson.annotation.JsonProperty;


public class ClipDatasetParams {

    @JsonProperty("INPUT_NAME")
    private String inputName;

    @JsonProperty("OUTPUT_NAME")
    private String outputName;

    @JsonProperty("bounds")
    private String bounds;

    @JsonProperty("FOLDER_ID")
    private Long folderId;

    public String getInputName() {
        return inputName;
    }

    public void setInputName(String inputName) {
        this.inputName = inputName;
    }

    public String getOutputName() {
        return outputName;
    }

    public void setOutputName(String outputName) {
        this.outputName = outputName;
    }

    public String getBounds() {
        return bounds;
    }

    public void setBounds(String bounds) {
        this.bounds = bounds;
    }

    public Long getFolderId() {
        return folderId;
    }

    public void setFolderId(Long folderId) {
        this.folderId = folderId;
    }

    @Override
    public String toString() {
        return "ClipDatasetParams{" +
                "inputName='" + inputName + '\'' +
                ", outputName='" + outputName + '\'' +
                ", bounds='" + bounds + '\'' +
                ", folderId='" + folderId + '\'' +
                '}';
    }
}
