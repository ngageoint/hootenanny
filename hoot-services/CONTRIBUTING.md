This file includes documentation for updates to services code to do things like...

1. add/remove advanced options exposed in the hoot2x ui
2. integrate new conflation types in the hoot2x ui

### Update the advanced options exposed in the hoot2x ui

1. Edit the `conf/services/conflationTypes.json` file

- add a new key/value pair for a given advanced option in the desired `${option}:members` property.
- the key is the actual hoot option flag found in the `conf/core/ConfigOptions.ascii` file, the value is the display name shown in the ui
- the `${option}` is the section in the ui the option will be placed. for example, if I have an option related to highways, it would make sense to add it in the `roads:members` property.
- for example, to expose the `highway.matcher.max.angle` option, an edit to the `conflationTypes.json` might be...

```javascript
...
 "Roads": {
        "matcher": "hoot::HighwayMatchCreator",
        "merger": "hoot::HighwayMergerCreator",
        "members": {
            "search.radius.highway": "Highway search radius",
            "highway.merge.tags.only": "Merge tags only",
            "highway.matcher.max.angle": "Maximum highway match angle" // new option...
        }
...
```

- Since cleaning options are members of the `map.cleaner.transform` option's list value, add the members of that list that a user should be able to toggle on and off
- for example, if it is helpful to turn on an off the `hoot::RemoveDuplicateAreaVisitor` cleaning option, an update might be...

```javascript
...
   "Cleaning": {
        "members": {
            "hoot::DualHighwaySplitter": "Split Dual Way",
            "hoot::RemoveDuplicateAreaVisitor": "Remove Duplicate Area" // new map cleaner...
        }
...
```

- these cleaning options will shown as checked by default in the ui

### Expose a new conflation type to the hoot2x ui

The Conflation type are driven by configs in `conf/core` directory. These configs are used to

1. Allow a user to select a conflation type like attribute conflation and have the actual hoot command run on the services' machine something like

```bash
hoot conflate ${reference} ${secondary} -C AttributeConflation.conf # any other command line arguments...
```

2. Ensure that the hoot2x advanced options panel options' state match what the conflation option configs define.

- For example, if a conflation type were to set `highway.merge.tags.only=true`, then the ui should show its checkbox as 'checked' when that conflation type is selected. Conversely, that checkbox should be unchecked in all cases where conflation types do not change the option's default value (in this case false).


The services code has the logic to read in these files and generate a lookup table used by the ui code to ensure this synced state. Contributors can integrate new conflation types in this lookup table by making 3 changes to service code.

1. update the `hoot-services/main/resources/conf/hoot-services.conf.in` file to include a reference to the new conflation config

- for example something like...

```
# new conflation
newConflationConf=conf/core/newConflation.conf
```

2. update the `hoot-services/src/main/java/hoot/services/HootProperties.java` with a new constant for the new conflationType

```java
public static final String NEW_CONFLATION_PATH; // define the new constant
```

```java
NEW_CONFLATION_PATH = getProperty("newConflationConf") // read the config into memory...
```

3. add a reference to the new conflation type to the HashMap of conf paths in the `hoot-services/src/main/java/hoot/services/controllers/conflation/AdvancedConflationOptionsResource.java`

```java
    private static Map<String, String> confMap = new HashMap<String, String>(){{
        put("attribute", ATTRIBUTE_CONFLATION_PATH);
        put("horizontal", HORIZONTAL_CONFLATION_PATH);
        put("network", NETWORK_CONFLATION_PATH);
        put("differntial", DIFFERENTIAL_CONFLATION_PATH);
        put("new", NEW_CONFLATION_PATH); // new option...
    }};
```


- To see these changes to a local instance of hoot during development run the following commands...

```bash
make services-build
$HOOT_HOME/scripts/tomcat/CopyWebAppsToTomcat.sh
sudo services tomcat8 restart
```
