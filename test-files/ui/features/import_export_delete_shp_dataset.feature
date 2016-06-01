Feature: Import Export Delete MGCP Shapefile Dataset

    Scenario: Opening Management Tab
        Given I am on Hootenanny
        And I click Get Started
        Then I select the "sprocket" div
        When I click on the "Datasets" option in the "settingsSidebar"

    Scenario: Import Dataset
        And I press "Add Dataset"
        And I select the "File (shp,zip)" option in the "Select Import Type" combobox
        And I select "/test-files/MGCPv3.zip" dataset
        Then I should see element "[id='importDatasetLayerName']" with value "MGCPv3"
        Then I type "mgcp_shp_cucumber" in input "importDatasetLayerName"
        Then I type "CucumberShp" in input "importDatasetNewFolderName"
        And I select the "Multinational Geospatial Co-production Program (MGCP) TRD3&4" option in the "Select Data Translation Schema" combobox
        When I press "Import" big loud span
        Then I wait 60 "seconds" to not see "Import"
        Then I wait 10 "seconds" to see "CucumberShp"

    Scenario: Export Dataset
        When I click the "CucumberShp" Dataset
        Then I click the "mgcp_shp_cucumber" Dataset
        And I context click the "mgcp_shp_cucumber" Dataset
        And I click the "Export" context menu item
        Then I should see element "[id='fileExportOutputName']" with value "mgcp_shp_cucumber"
        When I select the "Shapefile" option in the "File Geodatabase" combobox
        And I select the "Multinational Geospatial Co-production Program (MGCP) TRD3&4" option in the "NSG Topographic Data Store (TDS) v6.1" combobox
        And I press "Export" big loud span
        And I wait 3 "minutes" to not see "Output Name"
        Then the download file "mgcp_shp_cucumber.zip" should exist

    Scenario: Delete Dataset
        And I context click the "CucumberShp" Dataset
        And I click the "Delete" context menu item
        And I accept the alert
        Then I wait 30 "seconds" to not see "mgcp_shp_cucumber"
        Then I wait 1 "seconds" to not see "CucumberShp"
