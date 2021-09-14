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
 * @copyright Copyright (C) 2015, 2016, 2017, 2019, 2021 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.conflation;


import static hoot.services.HootProperties.ASCIIDOC_PATH;
import static hoot.services.HootProperties.ATTRIBUTE_CONFLATION_PATH;
import static hoot.services.HootProperties.ATT_OVERRIDE_PATH;
import static hoot.services.HootProperties.DIFFERENTIAL_CONFLATION_PATH;
import static hoot.services.HootProperties.HOME_FOLDER;
import static hoot.services.HootProperties.HOOT2_OVERRIDE_PATH;
import static hoot.services.HootProperties.HORIZONTAL_CONFLATION_PATH;
import static hoot.services.HootProperties.HORZ_OVERRIDE_PATH;
import static hoot.services.HootProperties.NETWORK_CONFLATION_PATH;
import static hoot.services.HootProperties.REF_OVERRIDE_PATH;
import static hoot.services.HootProperties.TEMPLATE_PATH;

import hoot.services.command.CommandResult;
import hoot.services.controllers.info.HootWaySnapCriteria;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.stream.Collectors;

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.apache.commons.io.FileUtils;
import org.apache.commons.lang3.StringUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;

import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;


@Controller
@Path("/advancedopts")
public class AdvancedConflationOptionsResource {
    private static final Logger logger = LoggerFactory.getLogger(AdvancedConflationOptionsResource.class);

    private JSONObject doc;
    private JSONArray template;
    private JSONArray referenceTemplate;
    private JSONArray horizontalTemplate;
    private JSONArray attributeTemplate;
    private JSONArray hoot2Template;
    private JSONArray diffTemplate;
    private JSONObject conflationOptionsTemplate;
    private JSONObject referenceOverride;
    private JSONObject horizontalOverride;
    private JSONObject attributeOverride;
    private JSONObject hoot2Override;

    private static Map<String, String> confMap = new HashMap<String, String>(){{
        put("attribute", ATTRIBUTE_CONFLATION_PATH);
        put("horizontal", HORIZONTAL_CONFLATION_PATH);
        put("network", NETWORK_CONFLATION_PATH);
        put("differential", DIFFERENTIAL_CONFLATION_PATH);
        put("differential w/Tags", DIFFERENTIAL_CONFLATION_PATH);
    }};

    private Map<String, String> matcherMap = new HashMap<String, String>(){{
        put("GenericLines", "hoot::LinearCriterion");
        put("PowerLines", "hoot::PowerLineCriterion");
        put("Railways", "hoot::RailwayCriterion");
        put("Rivers", "hoot::RiverCriterion");
        put("Roads", "hoot::HighwayCriterion");
    }};

    private static Map<String, Map<String, String>> confOptionsMap = null;

    private static Map<String, Map<String, String>> buildConfOptionsMap() {
        Map<String, Map<String, String>> conflationOptions = new HashMap<>();

        // build up map of conflation config maps...
        ObjectMapper mapper = new ObjectMapper();
        TypeReference<Map<String, String>> confSchema = new TypeReference<Map<String, String>>(){};
        for (Entry<String, String> conf: confMap.entrySet()) {
            String file;
            try {
                file = FileUtils.readFileToString(new File(HOME_FOLDER, conf.getValue()), Charset.defaultCharset());
                // reduce config option map to only those entries within the list
                // of options exposed as part of the advanced options ui...
                Map<String, String> uiConfOptions = new HashMap<>();
                Map<String, String> confOptions = mapper.readValue(file, confSchema);

                for (Entry<String, String> confOptionEntry: confOptions.entrySet()) {
                    if (ConflateCommand.isUiOption(confOptionEntry.getKey())) {
                        uiConfOptions.put(ConflateCommand.getConfigKey(confOptionEntry.getKey()), confOptionEntry.getValue());
                    }
                }

                conflationOptions.put(conf.getKey().toLowerCase(), uiConfOptions);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        return conflationOptions;
    }

    public static List<String> getConflationTypes() {
        List<String> conflationTypes = new ArrayList<String>();
        conflationTypes.addAll(confMap.keySet().stream().map(type -> {
                return StringUtils.capitalize(type);
            }).collect(Collectors.toList())
        );

        conflationTypes.sort((p1, p2) -> p1.compareTo(p2));
        conflationTypes.add(0, "Reference");
        return conflationTypes;
    }

    public AdvancedConflationOptionsResource() {
        if (confOptionsMap == null) {
            confOptionsMap = buildConfOptionsMap();
        }
    }

    @GET
    @Path("/conflationtypes")
    @Produces(MediaType.APPLICATION_JSON)
    public Response conflationTypes() {
        JSONArray responseJSON = new JSONArray();
        responseJSON.addAll(getConflationTypes());
        return Response.ok(responseJSON.toJSONString()).build();
    }

    @GET
    @Path("/getoptions")
    @Produces(MediaType.APPLICATION_JSON)
    public Response getOptions(@QueryParam("conftype") String confType, @QueryParam("force") String isForce) {
        JSONArray template;
        try {
            // Force option should only be used to update options list by administrator
            Boolean doForce = Boolean.valueOf(isForce);

            getOverrides(doForce);

            if ((doc == null) || doForce) {
                doc = new JSONObject();
                parseAsciidoc();
            }

            JSONParser parser = new JSONParser();

            //This first conditional gives the non-default values inferred by
            //a specific conflation type so the UI can update the discrete values
            //exposed in the advanced options pane
            if (confType.equalsIgnoreCase("conflationOptions")) {
                if((conflationOptionsTemplate == null) || doForce) {
                    conflationOptionsTemplate = new JSONObject(confOptionsMap);
                }
                return Response.ok(conflationOptionsTemplate.toJSONString()).build();
            }
            //This one gets the different options exposed in the advanced options pane
            //grouped by category
            else if (confType.equalsIgnoreCase("hoot2")) {
                if ((hoot2Template == null) || doForce) {
                    hoot2Template = new JSONArray();
                    hoot2Template = (JSONArray) hoot2Override.get("hoot2");

                    for (Object memberObj : hoot2Template) {
                        JSONObject obj = (JSONObject) memberObj;
                        if (obj.get("name") != null && obj.get("name").equals("Differential")) {
                            addMemberData((JSONArray) obj.get("members"));
                        }

                    }
                }
                template = hoot2Template;
            }
            //This one gets the differential options from the hoot2 config
            else if (confType.equalsIgnoreCase("differential")) {
                if ((diffTemplate == null) || doForce) {
                    JSONArray hoot2Opts = (JSONArray) hoot2Override.get("hoot2");
                    JSONObject diffOpts = (JSONObject) hoot2Opts.stream().filter(config -> {
                                return ((JSONObject)config).get("name").equals("Differential");
                            }).findFirst().orElse(null);
                    diffTemplate = (JSONArray) diffOpts.get("members");

                    addMemberData(diffTemplate);

                    //Add dropdown for road algorithm
                    JSONObject roadObj = (JSONObject) hoot2Opts.stream().filter(config -> {
                        return ((JSONObject) config).get("name").equals("Roads");
                    }).findFirst().orElse(null);
                    if (roadObj != null) {
                        JSONArray roadOpts = (JSONArray) roadObj.get("members");
                        JSONObject roadAlg = (JSONObject) roadOpts.stream().filter(config -> {
                            return ((JSONObject) config).get("id").equals("RoadEngines");
                        }).findFirst().orElse(null);
                        diffTemplate.add(0, roadAlg);
                    }
                }
                template = diffTemplate;
            }
            //The rest are used in Hoot1 UI
            else if (confType.equalsIgnoreCase("reference")) {
                if ((referenceTemplate == null) || doForce) {
                    referenceTemplate = new JSONArray();
                    referenceTemplate.add(referenceOverride);
                }
                template = referenceTemplate;
            }
            else if (confType.equalsIgnoreCase("horizontal")) {
                if ((horizontalTemplate == null) || doForce) {
                    horizontalTemplate = new JSONArray();
                    horizontalTemplate.add(horizontalOverride);
                }
                template = horizontalTemplate;
            }
            else if (confType.equalsIgnoreCase("attribute")) {
                if ((attributeTemplate == null) || doForce) {
                    attributeTemplate = new JSONArray();
                    attributeTemplate.add(attributeOverride);
                }
                template = attributeTemplate;
            }
            else {
                if ((this.template == null) || doForce) {
                    try (FileReader fileReader = new FileReader(new File(HOME_FOLDER, TEMPLATE_PATH))) {
                        this.template = (JSONArray) parser.parse(fileReader);
                        generateRule(this.template, null);
                    }
                }
                template = this.template;
            }
        }
        catch (Exception e) {
            String msg = "Error getting advanced options!  Cause: " + e.getMessage();
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return Response.ok(template.toJSONString()).build();
    }

    private void addMemberData(JSONArray template) {
        for (Object memberObj : template) {
            JSONObject obj = (JSONObject) memberObj;

            // add list of options for SnapUnconnectedWaysSnapCriteria
            if (obj.get("id") != null &&
                    (obj.get("id").equals("SnapUnconnectedWaysSnapCriteria") || obj.get("id").equals("DifferentialSecWayRemovalCriteria"))) {
                HootWaySnapCriteria hootCriteriaCommand = new HootWaySnapCriteria(this.getClass());
                CommandResult commandResult = hootCriteriaCommand.execute();
                String output = commandResult.getStdout().replace("\n", "");

                JSONArray data = new JSONArray();
                JSONObject displayToHootMap = new JSONObject();
                for (String criteria : output.split(";")) {
                    // add space between capital letters
                    String hootCriterion = criteria.replaceAll("([^_])([A-Z])", "$1 $2");
                    data.add(hootCriterion);
                    displayToHootMap.put(hootCriterion, criteria);
                }

                obj.replace("input", "multiCombobox");
                obj.put("data", data);
                obj.put("displayToHootMap", displayToHootMap);
                obj.put("matcherMap", matcherMap);
            }
        }
    }

    private void getOverrides(Boolean doForce) throws IOException, ParseException {
        JSONParser parser = new JSONParser();

        if ((hoot2Override == null) || doForce) {
            try (FileReader fileReader = new FileReader(new File(HOME_FOLDER, HOOT2_OVERRIDE_PATH))) {
                hoot2Override = (JSONObject) parser.parse(fileReader);
            }
        }

        if ((referenceOverride == null) || doForce) {
            try (FileReader fileReader = new FileReader(new File(HOME_FOLDER, REF_OVERRIDE_PATH))) {
                referenceOverride = (JSONObject) parser.parse(fileReader);
            }
        }

        if ((horizontalOverride == null) || doForce) {
            try (FileReader fileReader = new FileReader(new File(HOME_FOLDER, HORZ_OVERRIDE_PATH))) {
                horizontalOverride = (JSONObject) parser.parse(fileReader);
            }
        }

        if ((attributeOverride == null) || doForce) {
            try (FileReader fileReader = new FileReader(new File(HOME_FOLDER, ATT_OVERRIDE_PATH))) {
                attributeOverride = (JSONObject) parser.parse(fileReader);
            }
        }
    }

    private String getDepKeyVal(String sDefVal) {
        try {
            int iStart = sDefVal.indexOf('{') + 1;
            int iEnd = sDefVal.indexOf('}');
            String depKey = sDefVal.substring(iStart, iEnd).trim();

            if (!depKey.isEmpty()) {
                // now find the dep value
                JSONObject oDep = (JSONObject) doc.get(depKey);
                return oDep.get("Default Value").toString();
            }
        }
        catch (Exception e) {
            logger.debug(e.getMessage(), e);
        }

        return "";
    }

    private void generateRule(JSONArray jsonArray, JSONObject jsonObject) {
        // for each options in template apply the value
        for (Object o : jsonArray) {
            if (o instanceof JSONObject) {
                JSONObject curOpt = (JSONObject) o;
                // first check to see if there is key then apply the asciidoc val
                Object oKey = curOpt.get("hoot_key");
                if (oKey != null) {
                    String sKey = oKey.toString();
                    Object oDocKey = doc.get(sKey);
                    if (oDocKey != null) {
                        JSONObject jDocKey = (JSONObject) oDocKey;

                        Object oAttrib = jDocKey.get("Data Type");
                        if (oAttrib != null) {
                            String sDataType = oAttrib.toString().trim();
                            // We can not determine list vs multilist from
                            // asciidoc so we will skip for the data type
                            if (!sDataType.equals("list") && (!sDataType.isEmpty())) {
                                curOpt.put("elem_type", sDataType);
                                if (sDataType.equalsIgnoreCase("bool")) {
                                    // bool type is checkbox
                                    curOpt.put("elem_type", "checkbox");
                                }
                            }
                        }

                        oAttrib = jDocKey.get("Default Value");
                        if (oAttrib != null) {
                            String sDefVal = oAttrib.toString().trim();
                            if (!sDefVal.isEmpty()) {
                                // It is poting to other val
                                if (sDefVal.startsWith("$")) {
                                    String depDefVal = getDepKeyVal(sDefVal);
                                    curOpt.put("defaultvalue", depDefVal);
                                }
                                else {
                                    curOpt.put("defaultvalue", sDefVal);
                                }
                            }
                        }

                        oAttrib = jDocKey.get("Description");
                        if (oAttrib != null) {
                            String sDesc = oAttrib.toString().trim();
                            if (!sDesc.isEmpty()) {
                                curOpt.put("description", sDesc);
                            }
                        }

                        // handle override
                        oAttrib = curOpt.get("override");
                        if (oAttrib != null) {
                            JSONObject override = (JSONObject) oAttrib;

                            for (Object o1 : override.entrySet()) {
                                Map.Entry<Object, Object> pair = (Map.Entry<Object, Object>) o1;
                                curOpt.put(pair.getKey(), pair.getValue());
                            }
                            // remove override element
                            curOpt.remove("override");
                        }
                    }
                }
                else {
                    // Second check for hoot_val and if there is one then apply
                    // descripton from asciidoc.  This can be checked from parent object
                    Object oVal = curOpt.get("hoot_val");
                    if (oVal != null) {
                        String sVal = oVal.toString();
                        if (jsonObject != null) {
                            // parent always have to have hoot_key for hoot_val
                            Object pKey = jsonObject.get("hoot_key");
                            if (pKey != null) {
                                // try to get default list from parent
                                String sPKey = pKey.toString();
                                Object oDocPKey = doc.get(sPKey);
                                if (oDocPKey != null) {
                                    JSONObject jDocPKey = (JSONObject) oDocPKey;
                                    Object oDocDefList = jDocPKey.get("Default List");

                                    if (oDocDefList != null) {
                                        JSONObject defList = (JSONObject) oDocDefList;
                                        Iterator<Object> it = defList.entrySet().iterator();
                                        boolean hasNext = true;
                                        while (hasNext) {
                                            Map.Entry<Object, Object> pair = (Map.Entry<Object, Object>) it.next();
                                            hasNext = it.hasNext();
                                            if (pair.getKey().toString().equalsIgnoreCase(sVal)) {
                                                String sDefListDesc = pair.getValue().toString().trim();
                                                if (!sDefListDesc.isEmpty()) {
                                                    curOpt.put("description", sDefListDesc);
                                                    curOpt.put("defaultvalue", "true");
                                                }
                                                hasNext = false;
                                            }
                                        }
                                    }

                                    // If the parent is boolean then try to get default value
                                    Object oDocDataType = jDocPKey.get("Data Type");
                                    if ((oDocDataType != null) && oDocDataType.toString().equalsIgnoreCase("bool")) {
                                        Object oDocDefVal = jDocPKey.get("Default Value");
                                        if (oDocDefVal != null) {
                                            if (oDocDefVal.toString().equalsIgnoreCase("true")) {
                                                if (sVal.equalsIgnoreCase("true")) {
                                                    curOpt.put("isDefault", "true");
                                                }

                                                if (sVal.equalsIgnoreCase("false")) {
                                                    curOpt.put("isDefault", "false");
                                                }
                                            }

                                            if (oDocDefVal.toString().equalsIgnoreCase("false")) {
                                                if (sVal.equalsIgnoreCase("true")) {
                                                    curOpt.put("isDefault", "false");
                                                }

                                                if (sVal.equalsIgnoreCase("false")) {
                                                    curOpt.put("isDefault", "true");
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        // handle override
                        Object oAttrib = curOpt.get("override");
                        if (oAttrib != null) {
                            JSONObject override = (JSONObject) oAttrib;

                            for (Object o1 : override.entrySet()) {
                                Map.Entry<Object, Object> pair = (Map.Entry<Object, Object>) o1;
                                curOpt.put(pair.getKey(), pair.getValue());
                            }
                            // remove override element
                            curOpt.remove("override");
                        }
                    }
                }

                // now check for members and if one exist then recurse
                Object oValMembers = curOpt.get("members");
                if ((oValMembers instanceof JSONArray)) {
                    generateRule((JSONArray) oValMembers, curOpt);
                }
            }
        }

        // do recursion to visit each object and try to update value
    }

    // If line starts with "* " then attribute field
    // "Data Type:"
    // "Default Value:"
    // If line starts with "** " then list item field
    private String parseLine(String line, String curOptName) {
        String optName = curOptName;
        // If line starts with "=== " then it is option field
        if (line.startsWith("=== ")) {
            optName = line.substring(3).trim();
            JSONObject field = new JSONObject();
            doc.put(optName, field);
        }
        else {
            Object curObject = doc.get(optName);
            if (curObject != null) {
                JSONObject curOpt = (JSONObject) curObject;

                if (line.startsWith("* ")) {
                    String field = line.substring(1).trim();
                    String[] parts = field.split(":");
                    if (parts.length > 1) {
                        String k = parts[0];
                        String v = "";
                        for (int i = 1; i < parts.length; i++) {
                            if (!v.isEmpty()) {
                                v += ":";
                            }
                            v += parts[i].replaceAll("`", "");
                        }
                        curOpt.put(k.trim(), v.trim());
                    }
                }
                else if (line.startsWith("** ")) // must be list item
                {
                    // Try to get default list object for current option
                    String listKey = "Default List";
                    Object o = curOpt.get(listKey);
                    JSONObject defList = new JSONObject();
                    if (o != null) {
                        defList = (JSONObject) o;
                    }
                    else {
                        curOpt.put(listKey, defList); // create new if not exist
                    }

                    String curLine = line.substring(2).trim();

                    // try getting item description
                    String[] parts = curLine.split("` - ");
                    if (parts.length > 0) {
                        String k = parts[0].replaceAll("`", "").trim();
                        String v = "";
                        if (parts.length > 1) // there is description
                        {
                            for (int i = 1; i < parts.length; i++) {
                                if (!v.isEmpty()) {
                                    v += "` - ";
                                }
                                v += parts[i];
                            }
                        }
                        defList.put(k.trim(), v.trim());
                    }
                }
                else
                // must be description
                {
                    String curLine = line.trim();
                    if (!curLine.isEmpty()) {
                        String k = "Description";
                        Object o = curOpt.get(k);
                        if (o == null) {
                            curOpt.put(k, curLine);
                        }
                        else {
                            String v = curOpt.get(k) + " " + curLine;
                            curOpt.put(k, v.trim());
                        }
                    }
                }
            }
        }

        return optName;
    }

    private void parseAsciidoc() throws IOException {
        File file = new File(HOME_FOLDER, ASCIIDOC_PATH);
        if (!file.exists()) {
            throw new IOException("Missing required file: " + file.getAbsolutePath());
        }

        try (FileInputStream fileInputStream = new FileInputStream(file)) {
            try (InputStreamReader inputStreamReader = new InputStreamReader(fileInputStream)) {
                try (BufferedReader bufferedReader = new BufferedReader(inputStreamReader)) {
                    String line;
                    String currentOption = null;

                    // Read File Line By Line
                    while ((line = bufferedReader.readLine()) != null) {
                        // Print the content on the console
                        currentOption = parseLine(line, currentOption);
                    }
                }
            }
        }
    }
}
