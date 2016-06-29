Feature: Import Export Delete OSM Dataset

    Scenario: Opening Management Tab
        Given I am on Hootenanny
        And I click Get Started
        Then I select the "sprocket" div

    Scenario: Import Dataset
        When I click on the "Datasets" option in the "settingsSidebar"
        And I press "Add Dataset"
        And I select the "File (osm)" option in the "Select Import Type" combobox
        And I select "/test-files/dcpoi_clip.osm" dataset
        Then I should see element "[id='importDatasetLayerName']" with value "dcpoi_clip"
        When I press "big.loud" span with text "Import"
        Then I wait 30 "seconds" to see "dcpoi_clip"

    Scenario: Export Dataset
        When I click the "dcpoi_clip" Dataset
        And I context click the "dcpoi_clip" Dataset
        And I click the "Export" context menu item
        Then I should see element "[id='fileExportOutputName']" with value "dcpoi_clip"
        When I select the "Open Street Map (OSM)" option in the "File Geodatabase" combobox
        And I press "big.loud" span with text "Export"
        And I wait 30 seconds
        Then the download file "dcpoi_clip.zip" should exist

    Scenario: Create Folder
        And I press "Add Folder"
        Then I type "CucumberOsm" in input ""
        And I press "big.loud" span with text "Add Folder"

    Scenario: Move Dataset
        When I click the "dcpoi_clip" Dataset
        And I context click the "dcpoi_clip" Dataset
        And I click the "Move" context menu item
        And I select the "CucumberOsm" option in the "root" combobox
        And I press "big.loud" span with text "Update"

    Scenario: Rename Dataset
        When I click the "CucumberOsm" Dataset
        And I click the "dcpoi_clip" Dataset
        And I context click the "dcpoi_clip" Dataset
        And I click the "Rename" context menu item
        Then I type "dcpoi_clip_rename" in input "dcpoi_clip"
        And I press "big.loud" span with text "Update"

    Scenario: Delete Folder and Dataset
        When I context click the "CucumberOsm" Dataset
        And I click the "Delete" context menu item
        And I accept the alert
        Then I wait 30 "seconds" to not see "CucumberOsm"