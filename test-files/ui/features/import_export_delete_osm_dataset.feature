Feature: Import Export Delete OSM Dataset

    Scenario: Opening Management Tab
        Given I am on Hootenanny
        And I click Get Started
        Then I select the "sprocket" div

    Scenario: Import Dataset
        When I click on the "Datasets" option in the "settingsSidebar"
        And I press "Import Single Dataset"
        And I select the "File (osm,osm.zip,pbf)" option in the "Select Import Type" combobox
        And I select "/test-files/dcpoi_clip.osm" dataset
        Then I should see element "[id='importDatasetLayerName']" with value "dcpoi_clip"
        Then I type "dcpoi_clip_Cucumber" in input "importDatasetLayerName"
        When I press "big.loud" span with text "Import"
        Then I wait 30 "seconds" to see "dcpoi_clip_Cucumber"

    Scenario: Import PBF Dataset
        When I click on the "Datasets" option in the "settingsSidebar"
        And I press "Import Single Dataset"
        And I select the "File (osm,osm.zip,pbf)" option in the "Select Import Type" combobox
        And I select "/test-files/ToyTestCombined.pbf" dataset
        Then I should see element "[id='importDatasetLayerName']" with value "ToyTestCombined"
        Then I type "ToyTestCombined_Cucumber" in input "importDatasetLayerName"
        When I press "big.loud" span with text "Import"
        Then I wait 30 "seconds" to see "ToyTestCombined_Cucumber"

    Scenario: Export Dataset
        When I click the "dcpoi_clip_Cucumber" Dataset
        And I context click the "dcpoi_clip_Cucumber" Dataset
        And I click the "Export" context menu item
        Then I should see element "[id='fileExportOutputName']" with value "dcpoi_clip_Cucumber"
        When I select the "Open Street Map (OSM)" option in the "File Geodatabase" combobox
        And I press "big.loud" span with text "Export"
        And I wait 30 seconds
        Then the download file "dcpoi_clip_Cucumber.zip" should exist

    Scenario: Export PBF Dataset
        When I click the "ToyTestCombined_Cucumber" Dataset
        When I click the "dcpoi_clip_Cucumber" Dataset
        And I context click the "dcpoi_clip_Cucumber" Dataset
        And I click the "Export" context menu item
        Then I should see element "[id='fileExportOutputName']" with value "dcpoi_clip_Cucumber"
        When I select the "Open Street Map (PBF)" option in the "File Geodatabase" combobox
        And I press "big.loud" span with text "Export"
        And I wait 30 seconds
        Then the download file "dcpoi_clip_Cucumber.osm.pbf" should exist

    Scenario: Create Folder
        # Clean up from previous failed tests
        When I delete any existing "CucumberOsm" folder if necessary
        And I press "Add Folder"
        Then I type "CucumberOsm" in input ""
        And I press "big.loud" span with text "Add Folder"

    Scenario: Move Dataset
        When I click the "dcpoi_clip_Cucumber" Dataset
        And I context click the "dcpoi_clip_Cucumber" Dataset
        And I click the "Move" context menu item
        And I select the "CucumberOsm" option in the "root" combobox
        And I press "big.loud" span with text "Update"

    Scenario: Move PBF Dataset
        When I click the "ToyTestCombined_Cucumber" Dataset
        And I context click the "ToyTestCombined_Cucumber" Dataset
        And I click the "Move" context menu item
        And I select the "CucumberOsm" option in the "root" combobox
        And I press "big.loud" span with text "Update"

    Scenario: Rename Dataset
        When I click the "CucumberOsm" Dataset
        And I click the "dcpoi_clip_Cucumber" Dataset
        And I context click the "dcpoi_clip_Cucumber" Dataset
        And I click the "Rename" context menu item
        Then I type "dcpoi_clip_Cucumber_rename" in input "dcpoi_clip_Cucumber"
        And I press "big.loud" span with text "Update"

    Scenario: Delete Folder and Dataset
        When I context click the "CucumberOsm" Dataset
        And I click the "Delete" context menu item
        And I accept the alert
        Then I wait 30 "seconds" to not see "CucumberOsm"
