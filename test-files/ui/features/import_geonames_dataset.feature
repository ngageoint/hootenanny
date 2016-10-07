Feature: Import Geonames Dataset

    Scenario: Opening Management Tab
        Given I am on Hootenanny
        And I click Get Started
        Then I select the "sprocket" div
        When I click on the "Datasets" option in the "settingsSidebar"

    Scenario: Import Dataset with geonames file extension
        And I press "Import Single Dataset"
        # Here it doesn't matter which "File" option you pick
        And I select the "File (geonames,txt)" option in the "Select Import Type" combobox
        And I select "/test-files/cmd/glacial/ConvertGeoNames.geonames" dataset
        Then I should see element "[id='importDatasetLayerName']" with value "ConvertGeoNames"
        Then I type "geonames_Cucumber" in input "importDatasetLayerName"
        When I press "big.loud" span with text "Import"
        Then I wait 30 "seconds" to see "geonames_Cucumber"

    Scenario: Delete Dataset with geonames file extension
        And I click the "geonames_Cucumber" Dataset
        And I context click the "geonames_Cucumber" Dataset
        And I click the "Delete" context menu item
        And I accept the alert
        Then I wait 30 "seconds" to not see "geonames_Cucumber"

    Scenario: Import Dataset with txt file extension
        And I press "Import Single Dataset"
        And I select the "File (geonames,txt)" option in the "Select Import Type" combobox
        And I select "/test-files/cmd/glacial/ConvertGeoNames.txt" dataset
        Then I should see element "[id='importDatasetLayerName']" with value "ConvertGeoNames"
        Then I type "geonames_txt_Cucumber" in input "importDatasetLayerName"
        When I press "big.loud" span with text "Import"
        Then I wait 30 "seconds" to see "geonames_txt_Cucumber"

    Scenario: Delete Dataset with geonames file extension
        And I click the "geonames_txt_Cucumber" Dataset
        And I context click the "geonames_txt_Cucumber" Dataset
        And I click the "Delete" context menu item
        And I accept the alert
        Then I wait 30 "seconds" to not see "geonames_txt_Cucumber" 
