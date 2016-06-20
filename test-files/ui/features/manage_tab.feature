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
            | WFS Exports |
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
        Then I click on the "WFS Exports" option in the "settingsSidebar"
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
        Then I select the "sprocket" div
