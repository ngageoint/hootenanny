Feature: Manage Tab

    Scenario: Opening Management Tab
        Given I am on Hootenanny
        # And I resize the window
        And I click Get Started
        When I select the "sprocket" div
        Then I should see options in this order:
            | Settings |
            | Datasets |
            | Translation Assistant |
            | Translations |
            #| WFS Exports |
            | Basemaps |
            | Log |
            | Reports |
            | Review Bookmarks |
            | About |

    Scenario: Checking content of tabs
        When I click on the "Datasets" option in the "settingsSidebar"
        And I should see "Add Dataset"
        And I should see "Add Folder"
        Then I click on the "Translation Assistant" option in the "settingsSidebar"
        And I should see "Upload file(s)"
        And I should see "Upload folder"
        And I should see "Tag Schema"
        Then I click on the "Translations" option in the "settingsSidebar"
        And I should see "Add New Translation"
        #Then I click on the "WFS Exports" option in the "settingsSidebar"
        Then I click on the "Basemaps" option in the "settingsSidebar"
        And I should see "New Basemap"
        Then I click on the "Log" option in the "settingsSidebar"
        And I should see "Export Full Log"
        And I should see "Refresh"
        And I should see "Server Log"
        And I should see "UI Log"
        Then I click on the "Reports" option in the "settingsSidebar"
        Then I click on the "About" option in the "settingsSidebar"
        And I should see "Hootenanny Core - Version:"
        And I should see "Hootenanny Web Services - Version:"
        And I should see "Hootenanny iD "

    Scenario: Testing Click Events of Dataset Tab
        When I click on the "Datasets" option in the "settingsSidebar"
        # Clean up from previous failed tests
        When I delete any existing "ManageTabTest" folder if necessary
        When I delete any existing "TestFolder" folder if necessary
        # Add a new folder
        And I press "Add Folder"
        Then I fill "NewFolderName" input with "ManageTabTest"
        And I press "big.loud" span with text "Add Folder"
        # Move datasets into folder
        When I click the "AllDataTypesACucumber" Dataset and the "AllDataTypesBCucumber" Dataset
        And I context click the "AllDataTypesACucumber" Dataset
        And I should see "Delete (2)"
        Then I click the "Move (2)" context menu item
        And I select the "ManageTabTest" option in the "root" combobox
        And I press "big.loud" span with text "Update"
        # Move datasets back out of folder
        Then I click the "ManageTabTest" Dataset
        When I click the "AllDataTypesBCucumber" Dataset and the "AllDataTypesACucumber" Dataset
        And I context click the "AllDataTypesACucumber" Dataset
        And I should see "Delete (2)"
        Then I click the "Move (2)" context menu item
        And I press "big.loud" span with text "Update"
        # Rename the folder
        Then I wait 5 "seconds" to see "tspan" element with text "ManageTabTest"
        Then I click the "ManageTabTest" Dataset
        And I context click the "ManageTabTest" Dataset
        And I click the "Rename/Move ManageTabTest" context menu item
        Then I fill "fileOutputName" input with "TestFolder"
        And I press "big.loud" span with text "Update"
        # Add a sub folder
        Then I wait 5 "seconds" to see "tspan" element with text "TestFolder"
        Then I context click the "TestFolder" Dataset
        And I click the "Add Folder" context menu item
        Then I fill "NewFolderName" input with "TestSubFolder"
        And I press "big.loud" span with text "Add Folder"
        Then I wait 30 "seconds" to not see "Folder Name"
        # Add dataset into sub folder
        Then I expand the "TestFolder" folder
        Then I wait 5 "seconds" to see "tspan" element with text "TestSubFolder"
        And I click the "TestSubFolder" Dataset
        And I context click the "TestSubFolder" Dataset
        And I click the "Add Dataset" context menu item
        And I select the "File (osm,osm.zip,pbf)" option in the "Select Import Type" combobox
        And I select "/test-files/dcpoi_clip.osm" dataset
        And I fill "importDatasetLayerName" with "dcpoi_clip_Cucumber_manage"
        Then I should see element "[id='importDatasetLayerName']" with value "dcpoi_clip_Cucumber_manage"
        And I should see element "#importDatasetPathName" with no value and placeholder "TestFolder/TestSubFolder"
        When I press "big.loud" span with text "Import"
        Then I wait 30 "seconds" to not see "Import Type"

        # TODO: Rename dataset
        # TODO: Add as reference
        # TODO: Add as secondary

        # Delete sub folder and dataset
        And I click the "TestSubFolder" Dataset
        And I should see "dcpoi_clip_Cucumber_manage"
        And I context click the "TestSubFolder" Dataset
        And I click the "Delete" context menu item
        And I accept the alert
        And I should see "TestFolder"
        # Delete the folder
        And I click the "TestFolder" Dataset
        And I context click the "TestFolder" Dataset
        And I click the "Delete" context menu item
        And I accept the alert
