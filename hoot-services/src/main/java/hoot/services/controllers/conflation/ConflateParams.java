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
 * @copyright Copyright (C) 2015, 2016, 2017, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.conflation;


import java.util.List;
import java.util.Map;

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

    @JsonProperty("OUTPUT_FOLDER")
    private Long outputFolder;

    @JsonProperty("COLLECT_STATS")
    private Boolean collectStats;

    @JsonProperty("bounds")
    private String bounds;

    @JsonProperty("CONFLATION_TYPE")
    private String conflationType;

    @JsonProperty("CONFLATION_ALGORITHM")
    private String conflateAlgorithm;

    @JsonProperty("CONFLATION_COMMAND")
    private String conflationCommand;

    @JsonProperty("HOOT2_ADV_OPTIONS")
    private Map<String, String> hoot2AdvOptions;

    @JsonProperty("HOOT_2")
    private Boolean hoot2;

    @JsonProperty("DISABLED_FEATURES")
    private List<String> disabledFeatures;

    @JsonProperty("CLEANING_OPTIONS")
    private List<String> cleaningOpts;

    @JsonProperty("taskInfo")
    private String taskInfo;

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

    public Long getOutputFolder() {
        return outputFolder;
    }

    public void setOutputFolder(Long outputFolder) {
        this.outputFolder = outputFolder;
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

    public String getConflationCommand() {
        return conflationCommand;
    }

    public void setConflationCommand(String conflationCommand) {
        this.conflationCommand = conflationCommand;
    }

    public Map<String, String> getHoot2AdvOptions() {
        return this.hoot2AdvOptions;
    }

    public void setHoot2AdvOptions(Map<String, String> hoot2AdvOptions) {
        this.hoot2AdvOptions = hoot2AdvOptions;
    }

    public void setHoot2(boolean hoot2) {
        this.hoot2 = hoot2;
    }

    public Boolean getHoot2() {
        return this.hoot2;
    }

    public String getConflateAlgorithm() {
        return this.conflateAlgorithm;
    }

    public void setConflateAlgorithm(String algorithm) {
        this.conflateAlgorithm = algorithm;
    }

    public List<String> getDisabledFeatures() {
        return this.disabledFeatures;
    }

    public void setDisabledFeatures(List<String> disabledFeatures) {
        this.disabledFeatures = disabledFeatures;
    }

    public List<String> getCleaningOpts() {
        return this.cleaningOpts;
    }

    public void setCleaningOpts(List<String> cleaningOpts) {
        this.cleaningOpts = cleaningOpts;
    }

    public void setTaskInfo(String taskInfo) {
        this.taskInfo = taskInfo;
    }

    public String getTaskInfo() {
        return taskInfo;
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
                ", outputFolder='" + outputFolder + '\'' +
                ", collectStats=" + collectStats +
                ", bounds='" + bounds + '\'' +
                ", conflationType='" + conflationType + '\'' +
                ", conflationCommand='" + conflationCommand + '\'' +
                '}';
    }

}
