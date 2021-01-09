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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.conflation;

import static hoot.services.HootProperties.CONFLATION_TYPES_PATH;
import static hoot.services.HootProperties.HOME_FOLDER;
import static hoot.services.HootProperties.HOOTAPI_DB_URL;
import static hoot.services.HootProperties.NETWORK_CONFLATION_PATH;
import static hoot.services.HootProperties.RPT_STORE_PATH;

import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import org.apache.commons.io.FileUtils;

import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import hoot.services.command.CommandResult;
import hoot.services.command.ExternalCommand;
import hoot.services.models.db.Users;


class ConflateCommand extends ExternalCommand {

    private final ConflateParams conflateParams;

    private static final List<String> conflationAlgorithms = Arrays.asList("Network");

    private String conflationType = null;
    private String conflationAlgorithm = null;
    private static List<String> cleaningOptions = new ArrayList<>();

    private static Map<String, Map<String, Object>> conflationFeatures = null;

    static {
        try {
            String file = FileUtils.readFileToString(new File(HOME_FOLDER, CONFLATION_TYPES_PATH), Charset.defaultCharset());
            ObjectMapper mapper = new ObjectMapper();
            TypeReference<?> schema = new TypeReference<Map<String, Map<String, Object>>>(){};
            conflationFeatures = mapper.readValue(file, schema);

            // use default options for map cleaners list...
            cleaningOptions.addAll(Arrays.asList(configOptions.get("MapCleanerTransforms").get("default")
                .replaceAll("hoot::","").split(";")));

        } catch (IOException e) {
            e.printStackTrace();
        }
    }


    public static String getConfigKey(String key) {
        String configKey = null;
        for (String k: configOptions.keySet()) {
             if (configOptions.get(k).get("key").equals(key)) {
                 configKey = k;
                  break;
             }
        }
        return configKey;
    }

    public static boolean isUiOption(String key) {
        boolean isOption = false;
        for (String k: conflationFeatures.keySet()) {
            if (((HashMap<?, ?>) conflationFeatures.get(k).get("members")).keySet().contains(key)) {
                isOption = true;
                break;
            }
        }
        return isOption;
    }


    ConflateCommand(String jobId, ConflateParams params, String debugLevel, Class<?> caller, Users user) throws IllegalArgumentException {
        super(jobId);
        this.conflateParams = params;

        List<String> options = new LinkedList<>();
        options.add("writer.include.conflate.score.tags=false");
        options.add("hootapi.db.writer.overwrite.map=true");
        options.add("writer.text.status=true");
        options.add("job.id=" + jobId);
        if(user == null) {
            options.add("api.db.email=test@test.com");
        } else {
            options.add("api.db.email=" + user.getEmail());
        }

        String input1Type = params.getInputType1();
        String input1 = input1Type.equalsIgnoreCase("DB") ? (HOOTAPI_DB_URL + "/" + params.getInput1()) : params.getInput1();

        String input2Type = params.getInputType2();
        String input2 = input2Type.equalsIgnoreCase("DB") ? (HOOTAPI_DB_URL + "/" + params.getInput2()) : params.getInput2();

        String output = HOOTAPI_DB_URL + "/" + params.getOutputName();

        String stats = "";
        if (params.getCollectStats()) {
            //Hootenanny map statistics such as node and way count
            stats = "--stats";
        }

        if (params.getBounds() != null) {
            //Add bounds
            options.add("bounds=" + params.getBounds());
        }

        Map<String, Object> substitutionMap = new HashMap<>();

        // Detect Differential Conflation
        String conflationCommand = params.getConflationCommand();
        String diffTags = "";
        String differential = "";
        if (conflationCommand != null && conflationCommand.contains("differential-tags")){
          conflationCommand = "conflate";
          differential = "--differential";
          diffTags = "--include-tags";
        }
        else if (conflationCommand != null && conflationCommand.contains("differential")){
          conflationCommand = "conflate";
          differential = "--differential";
        }

        if (params.getAdvancedOptions() != null && !params.getAdvancedOptions().isEmpty()) { // hoot 1
            substitutionMap.put("CONFLATION_TYPE", "");
            Arrays.stream(toOptionsList(params.getAdvancedOptions())).forEach((option) -> {
                if (!option.isEmpty()) {
                    options.add(option.trim());
                }
            });
        }

        //Attribute Reference Layer is not default
        if ("2".equals(params.getReferenceLayer())) {
            options.add("tag.merger.default=hoot::OverwriteTag1Merger");
        }

        if (params.getHoot2() != null) { // hoot 2
            conflationType = params.getConflationType();
            conflationAlgorithm = params.getConflateAlgorithm();
            List<String> disabledFeatures = params.getDisabledFeatures();

            if (disabledFeatures != null && !disabledFeatures.isEmpty()) {
                List<String> matchers = new ArrayList<>();

                String roadType = "Network".equals(conflationAlgorithm) ? "Roads" : "RoadsNetwork";
                for (String feature: conflationFeatures.keySet()) {
                    // ignore the road matcher/mergers not relevant conflation type/algorithm confs...
                    if (feature.equals(roadType)) continue;
                    // ignore conflationFeatures that don't have a matcher and merger
                    Map<String, Object> conflationFeature = conflationFeatures.get(feature);
                    String conflationMatcher = (String) conflationFeature.get("matcher");
                    String conflationMerger = (String) conflationFeature.get("merger");
                    if (conflationMatcher != null && !conflationMatcher.isEmpty()
                            && conflationMerger != null && !conflationMerger.isEmpty()
                            && !disabledFeatures.contains(feature)) {
                        matchers.add(conflationMatcher);
                    }
                }

                // These 2 lists will be in the proper order so we will us this to sort our matchers and mergers list
                List<String> matchCreators;
                List<String> mergerCreators;

                // SPECIAL CASE: if network, use order networkAlgorithm.conf
                if ("Network".equals(conflationType) || "Network".equals(conflationAlgorithm)) {
                    JSONParser parser = new JSONParser();
                    try (FileReader fileReader = new FileReader(new File(HOME_FOLDER, NETWORK_CONFLATION_PATH))) {
                        JSONObject networkConfigJson = (JSONObject) parser.parse(fileReader);
                        matchCreators = getCreatorsFromJson("match.creators", "MatchCreators", networkConfigJson);
                        mergerCreators = getCreatorsFromJson("merger.creators", "MergerCreators", networkConfigJson);
                    }
                    catch (IOException | ParseException ioe) {
                        throw new RuntimeException("Error reading NetworkAlgorithm.conf file", ioe);
                    }
                } else {
                    // These 2 lists are in the proper order so we will use this to sort our matchers and mergers list
                    matchCreators = new ArrayList<>(Arrays.asList(configOptions.get("MatchCreators").get("default").split(";")));
                    mergerCreators = new ArrayList<>(Arrays.asList(configOptions.get("MergerCreators").get("default").split(";")));
                }

                List<String> sortedMatchers = new ArrayList<>();
                List<String> sortedMergers = new ArrayList<>();
                // since we are looping through the configOptions matchers list, we can check if our 'matchers' list we added items to contains the same strings.
                // if it does, we just add those items from the configOptions matchers, along with the respective index item from configOptions mergers, to maintain proper order.
                for (int i = 0; i < matchCreators.size(); i++) {
                    String current = matchCreators.get(i);

                    // Special case for network
                    if (current.equals("hoot::NetworkMatchCreator") && (matchers.contains("hoot::HighwayMatchCreator") || matchers.contains("hoot::NetworkMatchCreator"))) {
                        sortedMatchers.add("hoot::NetworkMatchCreator");
                        sortedMergers.add("hoot::NetworkMergerCreator");
                    }
                    else if (matchers.contains(current)) {
                        sortedMatchers.add(current);
                        sortedMergers.add(mergerCreators.get(i));
                    }
                }

                options.add("match.creators=" + String.join(";", sortedMatchers));
                options.add("merger.creators=" + String.join(";", sortedMergers));
            }

            Map<String, String> hoot2AdvOptions = params.getHoot2AdvOptions();

            if (hoot2AdvOptions != null && !hoot2AdvOptions.isEmpty()) {
                for (Entry<String, String> option: hoot2AdvOptions.entrySet()) {
                    if (configOptions.containsKey(option.getKey())) { // if option key in possible values, add new option command
                        Map<String, String> optionConfig = configOptions.get(option.getKey());
                        String optionValue = option.getValue();

                        if (optionConfig.get("type").toLowerCase().equals("list")) {
                            optionValue = optionValue.replaceAll("\\[|\\]", "").replaceAll(",", ";");
                        }

                        // if value is json string
                        if (optionValue.matches("\\{(.*)\\}")) {
                            options.add(optionConfig.get("key") + "=" + optionValue);
                        } else {
                            options.add("\"" + optionConfig.get("key") + "=" + optionValue + "\"");
                        }
                    }
                }
            }

            if (params.getCleaningOpts() != null) { // remove cleaning specified cleaning options...
                for (String cleaningOption: params.getCleaningOpts()) {
                    if (cleaningOptions.contains(cleaningOption)) {
                        options.add("\"map.cleaner.transforms-=hoot::" + cleaningOption + "\"");
                    }
                }
            }

        }

        substitutionMap.put("CONFLATION_COMMAND", conflationCommand);
        substitutionMap.put("DEBUG_LEVEL", debugLevel);
        substitutionMap.put("CONFLATION_TYPE", "");
        substitutionMap.put("CONFLATION_ALGORITHM", "");
        substitutionMap.put("HOOT_OPTIONS", toHootOptions(options));
        substitutionMap.put("INPUT1", input1);
        substitutionMap.put("INPUT2", input2);
        substitutionMap.put("OUTPUT", output);
        substitutionMap.put("DIFFERENTIAL", differential);
        substitutionMap.put("DIFF_TAGS", diffTags);
        substitutionMap.put("STATS", stats);

        String command;
        if (params.getHoot2() == null) { // hoot1
            command = "hoot.bin ${CONFLATION_COMMAND} --${DEBUG_LEVEL} ${HOOT_OPTIONS} ${INPUT1} ${INPUT2} ${OUTPUT} ${DIFFERENTIAL} ${DIFF_TAGS} ${STATS}";
        } else {

            if (conflationType != null) {
                if (AdvancedConflationOptionsResource.getConflationTypes().stream().noneMatch(t -> t.equals(conflationType))) {
                    throw new IllegalArgumentException(String.format("Conflation Type \"%s\" is not valid.", conflationType));
                }
                //Differential w/Tags maps to Differential
                if (conflationType.equals("Differential w/Tags")) {
                    conflationType = "Differential";
                }

                //Network maps to a conflation algorithm
                if (conflationType.equals("Network")) {
                    if (conflationAlgorithm == null)
                        conflationAlgorithm = "Network";
                    conflationType = null;

                } else {
                    substitutionMap.put("CONFLATION_TYPE", conflationType + "Conflation.conf");
                }
            }

            if (conflationAlgorithm != null) {
                if (conflationAlgorithms.stream().noneMatch(a -> a.equals(conflationAlgorithm))) {
                    throw new IllegalArgumentException(String.format("Conflation Algorithm \"%s\" is not valid.", conflationAlgorithm));
                }
                substitutionMap.put("CONFLATION_ALGORITHM", conflationAlgorithm + "Algorithm.conf");
            }

            command = "hoot.bin ${CONFLATION_COMMAND} --${DEBUG_LEVEL}"
                    + (conflationType != null ? " -C ${CONFLATION_TYPE}" : "")
                    + (conflationAlgorithm != null ? " -C ${CONFLATION_ALGORITHM}" : "")
                    + " ${HOOT_OPTIONS} ${INPUT1} ${INPUT2} ${OUTPUT} ${DIFFERENTIAL} ${DIFF_TAGS} ${STATS}";
        }

        super.configureCommand(command, substitutionMap, caller);
    }

    /*
     * Returns a list of conflate match or merger creators given a configuration
     *
     * @param optionName core configuration option name; e.g. "match.creators"
     * @param defaultOptionId configuration option name as referenced in the services default config; e.g. "MatchCreators"
     * @param config the JSON configuration to parse
     * @note Option replacement syntax is valid for any hoot config option, but in practice its only currently being used from the
     * match/merger creator options, so the concept or parsing it may have to be abstracted here at a later time.
     * @return a list of match/merger creator strings
     */
    private List<String> getCreatorsFromJson(String optionName, String defaultOptionId, JSONObject config) {
        // hoot json config files support an option list value entry replacement syntax. For instance, to replace an instance of
        // "hoot::HighwayMatchCreator", in the "match.creators" config option the json snippet would look like:
        //
        // "match.creators": "hoot::HighwayMatchCreator->hoot::NetworkMatchCreator",
        //
        // where '->' means replace the first item with the second item.

        String creatorsStr = config.get(optionName).toString();

        if (creatorsStr.contains("->")) {
            // If our string contains the replacement operator, parse through each entry and perform the option value entry replacement
            // on the default configuration. We're assuming that if one option val entry has '->', then they all do which is also what
            // the core does when parsing CLI args.

            List <String> modifiedCreators;
            List <String> defaultCreators =
                new ArrayList<>(Arrays.asList(configOptions.get(defaultOptionId).get("default").split(";")));
            modifiedCreators = defaultCreators;
            List <String> creatorReplacements = new ArrayList <>(Arrays.asList(creatorsStr.split(";")));
            for (int i = 0; i < creatorReplacements.size(); i++) {
                String creatorReplacement = creatorReplacements.get(i);
                List <String> creatorReplacementParts = new ArrayList <>(Arrays.asList(creatorReplacement.split("->")));
                if (creatorReplacementParts.size() != 2) {
                    throw new IllegalArgumentException(
                        String.format("Invalid replacement option value entry: \"%s\".", creatorReplacement));
                }
                String creatorToReplace = creatorReplacementParts.get(0);
                String replacementCreator = creatorReplacementParts.get(1);
                if (!modifiedCreators.contains(creatorToReplace)) {
                    throw new IllegalArgumentException(
                        String.format("Option value entry to replace does not exist: \"%s\".", creatorToReplace));
                }
                Collections.replaceAll(modifiedCreators, creatorToReplace, replacementCreator);
            }
            return modifiedCreators;
        } else {
            // Otherwise, just parse the creators as a literal option list value.
            return new ArrayList<>(Arrays.asList(creatorsStr.split(";")));
        }
    }

    private String[] toOptionsList(String optionsString) {
        return optionsString.trim().split("-D ");
    }

    @Override
    public CommandResult execute() {
        CommandResult commandResult = super.execute();

        if (conflateParams.getCollectStats()) {
            File statsFile = new File(RPT_STORE_PATH, conflateParams.getOutputName() + "-stats.csv");
            try {
                FileUtils.write(statsFile, commandResult.getStdout(), Charset.defaultCharset());
            }
            catch (IOException ioe) {
                throw new RuntimeException("Error writing to " + statsFile.getAbsolutePath(), ioe);
            }
        }

        return commandResult;
    }
}
