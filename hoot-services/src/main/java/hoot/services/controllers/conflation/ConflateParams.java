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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.conflation;


import com.fasterxml.jackson.annotation.JsonProperty;


public class ConflateParams {

    @JsonProperty("INPUT1_TYPE")
    private String inputType1;

    @JsonProperty("INPUT2_TYPE")
    private String inputType2;

    @JsonProperty("INPUT1")
    private String input1;

    @JsonProperty("INPUT2")
    private String input2;

    @JsonProperty("REFERENCE_LAYER")
    private String referenceLayer;

    @JsonProperty("ADV_OPTIONS")
    private String advancedOptions;

    @JsonProperty("OUTPUT_NAME")
    private String outputName;

    @JsonProperty("COLLECT_STATS")
    private Boolean collectStats;

    @JsonProperty("TASK_BBOX")
    private String bounds;

    @JsonProperty("CONFLATION_TYPE")
    private String conflationType;

    @JsonProperty("GENERATE_REPORT")
    private String generateReport;

    @JsonProperty("TIME_STAMP")
    private String timeStamp;

    @JsonProperty("USER_EMAIL")
    private String userEmail;

    @JsonProperty("CONFLATION_COMMAND")
    private String conflationCommand;

    public String getInputType1() {
        return inputType1;
    }

    public void setInputType1(String inputType1) {
        this.inputType1 = inputType1;
    }

    public String getInputType2() {
        return inputType2;
    }

    public void setInputType2(String inputType2) {
        this.inputType2 = inputType2;
    }

    public String getReferenceLayer() {
        return referenceLayer;
    }

    public void setReferenceLayer(String referenceLayer) {
        this.referenceLayer = referenceLayer;
    }

    public String getAdvancedOptions() {
        return advancedOptions;
    }

    public void setAdvancedOptions(String advancedOptions) {
        this.advancedOptions = advancedOptions;
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

    public String getOutputName() {
        return outputName;
    }

    public void setOutputName(String outputName) {
        this.outputName = outputName;
    }

    public Boolean getCollectStats() {
        return collectStats;
    }

    public void setCollectStats(Boolean collectStats) {
        this.collectStats = collectStats;
    }

    public String getBounds() {
        return bounds;
    }

    public void setBounds(String bounds) {
        this.bounds = bounds;
    }

    public String getConflationType() {
        return conflationType;
    }

    public void setConflationType(String conflationType) {
        this.conflationType = conflationType;
    }

    public String getGenerateReport() {
        return generateReport;
    }

    public void setGenerateReport(String generateReport) {
        this.generateReport = generateReport;
    }

    public String getTimeStamp() {
        return timeStamp;
    }

    public void setTimeStamp(String timeStamp) {
        this.timeStamp = timeStamp;
    }

    public String getUserEmail() {
        return userEmail;
    }

    public void setUserEmail(String userEmail) {
        this.userEmail = userEmail;
    }

    public String getConflationCommand() {
        return conflationCommand;
    }

    public void setConflationCommand(String conflationCommand) {
        this.conflationCommand = conflationCommand;
    }

    @Override
    public String toString() {
        return "ConflateParams{" +
                "inputType1='" + inputType1 + '\'' +
                ", inputType2='" + inputType2 + '\'' +
                ", input1='" + input1 + '\'' +
                ", input2='" + input2 + '\'' +
                ", referenceLayer='" + referenceLayer + '\'' +
                ", advancedOptions='" + advancedOptions + '\'' +
                ", outputName='" + outputName + '\'' +
                ", collectStats=" + collectStats +
                ", bounds='" + bounds + '\'' +
                ", conflationType='" + conflationType + '\'' +
                ", generateReport='" + generateReport + '\'' +
                ", timeStamp='" + timeStamp + '\'' +
                ", userEmail='" + userEmail + '\'' +
                ", conflationCommand='" + conflationCommand + '\'' +
                '}';
    }
}
