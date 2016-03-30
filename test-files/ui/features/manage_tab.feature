Feature: Manage Tab

    Scenario: Opening Management Tab
        Given I am on Hootenanny
        And I resize the window
        And I press "Get Started"
        When I select the "Manage" div
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
        Then I select the "Datasets" tab
        And I should see "Add Dataset"
        And I should see "Add Folder"
        Then I select the "Translation Assistant" tab
        And I should see "Upload file(s)"
        And I should see "Upload folder"
        And I should see "Tag Schema"
        Then I select the "Translations" tab
        And I should see "Add New Translation"
        Then I select the "WFS Exports" tab
        Then I select the "Basemaps" tab
        And I should see "New Basemap"
        Then I select the "Log" tab
        And I should see "Export Full Log"
        And I should see "Refresh"
        And I should see "Error Log"
        And I should see "UI Log"
        Then I select the "Reports" tab
        Then I select the "About" tab
        And I should see "Hootenanny Core - Version:"
        And I should see "Hootenanny Web Services - Version:"
        And I should see "Hootenanny iD Editor - Version:"
        Then I select the "Return to Map" div