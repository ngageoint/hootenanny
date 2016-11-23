Feature: Bulk Import, Directory Import, and Multiselect Delete Datasets

    Scenario: Opening Management Tab
        Given I am on Hootenanny
        And I click Get Started
        Then I select the "sprocket" div

    Scenario: Bulk Import Dataset
        When I click on the "Datasets" option in the "settingsSidebar"
        And I context click "Import Single Dataset"
        And I click the first "dataset-option" list item
        And I wait 5 "seconds" to see "Bulk Import Datasets"
        Then I press "big.loud" span with text "Add Row"
        Then I select in row 0 the "File (osm,osm.zip,pbf)" option in the "Select Import Type" combobox
        And I select in row 0 the "/test-files/dcpoi_clip.osm" dataset
        And I should see row 0 input "Save As" with value "dcpoi_clip"
        Then I fill row 0 input "Save As" with value "dcpoi_clip_bulkImport_Cucumber"
        Then I select in row 1 the "File (osm,osm.zip,pbf)" option in the "Select Import Type" combobox
        And I select in row 1 the "/test-files/mapcruzinpoi_clip.osm" dataset
        And I should see row 1 input "Save As" with value "mapcruzinpoi_clip"
        Then I fill row 1 input "Save As" with value "mapcruzinpoi_clip_bulkImport_Cucumber"
        Then I press "big.loud" span with text "Import"
        And I should see "Initializing..."
        And I wait 2 "minutes" to see "Close"
        Then I press "big.loud" span with text "Close"
        Then I wait 2 "minutes" to not see "Bulk Import Datasets"
        Then I wait 2 "minutes" to see "dcpoi_clip_bulkImport_Cucumber"
        And I wait 2 "minutes" to see "mapcruzinpoi_clip_bulkImport_Cucumber"

    Scenario: Bulk Import Dataset with custom suffix
        When I click on the "Datasets" option in the "settingsSidebar"
        And I context click "Import Single Dataset"
        And I click the first "dataset-option" list item
        And I wait 5 "seconds" to see "Bulk Import Datasets"
        Then I press "big.loud" span with text "Add Row"
        Then I select in row 0 the "File (osm,osm.zip,pbf)" option in the "Select Import Type" combobox
        And I select in row 0 the "/test-files/dcpoi_clip.osm" dataset
        And I should see row 0 input "Save As" with value "dcpoi_clip"
        Then I fill row 0 input "Save As" with value "dcpoi_clip"
        Then I select in row 1 the "File (osm,osm.zip,pbf)" option in the "Select Import Type" combobox
        And I select in row 1 the "/test-files/mapcruzinpoi_clip.osm" dataset
        And I should see row 1 input "Save As" with value "mapcruzinpoi_clip"
        Then I fill row 1 input "Save As" with value "mapcruzinpoi_clip"
        Then I fill "#customSuffix" with "_customSuffix"
        Then I press "big.loud" span with text "Import"
        And I should see "Initializing..."
        And I wait 2 "minutes" to see "Close"
        Then I press "big.loud" span with text "Close"        
        Then I wait 30 "seconds" to not see "Bulk Import Datasets"
        Then I wait 30 "seconds" to see "dcpoi_clip_customSuffix"
        And I wait 2 "minutes" to see "mapcruzinpoi_clip_customSuffix"

    Scenario: Import Directory with Custom Suffix
        When I click on the "Datasets" option in the "settingsSidebar"
        And I press "Import Directory"
        And I select the "OSM or PBF" option in the "Select Import Type" combobox
        And I select "AllDataTypesA.osm" and "AllDataTypesB.osm" from "/test-files/conflate/unified/" directory
        Then I type "_directoryImport" in input "importDirectoryCustomSuffix"
        When I press "big.loud" span with text "Import"
        Then I wait 60 "seconds" to not see "Cancel"
        When I press "big.loud" span with text "Close"
        Then I wait 10 "seconds" to see "AllDataTypesA_directoryImport"
        #Then I wait 10 "seconds" to see "AllDataTypesB_directoryImport"

    Scenario: Multiselect Delete
        When I click the "dcpoi_clip_bulkImport_Cucumber" Dataset and the "mapcruzinpoi_clip_bulkImport_Cucumber" Dataset
        And I context click the "mapcruzinpoi_clip_bulkImport_Cucumber" Dataset
        And I click the "Delete" context menu item
        And I wait
        And I accept the alert
        Then I should not see "dcpoi_clip_bulkImport_Cucumber" dataset after 30 "seconds"
        Then I should not see "mapcruzinpoi_clip_bulkImport_Cucumber" dataset after 30 "seconds"
        #
        When I click the "dcpoi_clip_customSuffix" Dataset and the "mapcruzinpoi_clip_customSuffix" Dataset
        And I context click the "mapcruzinpoi_clip_customSuffix" Dataset
        And I click the "Delete" context menu item
        And I wait
        And I accept the alert
        Then I should not see "dcpoi_clip_customSuffix" dataset after 30 "seconds"
        Then I should not see "mapcruzinpoi_clip_customSuffix" dataset after 30 "seconds"
        #When I click the "AllDataTypesA_directoryImport" Dataset  and the "AllDataTypesB_directoryImport" Dataset
        And I click the "AllDataTypesA_directoryImport" Dataset
        And I context click the "AllDataTypesA_directoryImport" Dataset
        And I click the "Delete" context menu item
        And I wait
        And I accept the alert
        Then I should not see "AllDataTypesA_directoryImport" dataset after 30 "seconds"
        # Then I should not see "AllDataTypesB_directoryImport" dataset after 30 "seconds"
