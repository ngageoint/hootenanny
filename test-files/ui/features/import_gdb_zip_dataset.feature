Feature: Import Zipped GDB Dataset

    Scenario: Opening Management Tab
        Given I am on Hootenanny
        And I click Get Started
        Then I select the "sprocket" div
        When I click on the "Datasets" option in the "settingsSidebar"

    Scenario: Import Dataset
        And I press "Import Single Dataset"
        And I select the "File (shp,zip,gdb.zip)" option in the "Select Import Type" combobox
        And I select "/test-files/gdb/DcGisRoads.gdb.zip" dataset
        Then I should see element "[id='importDatasetLayerName']" with value "DcGisRoads"
        Then I type "gdb_zip_Cucumber" in input "importDatasetLayerName"
        When I press "big.loud" span with text "Import"
        Then I wait 45 "seconds" to see "gdb_zip_Cucumber"

    Scenario: Delete Dataset
        And I click the "gdb_zip_Cucumber" Dataset
        And I context click the "gdb_zip_Cucumber" Dataset
        And I click the "Delete" context menu item
        And I accept the alert
        Then I wait 30 "seconds" to not see "gdb_zip_Cucumber"
