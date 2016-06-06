Feature: Log Export

    Scenario: Opening Management Tab
        Given I am on Hootenanny
        # And I resize the window
        And I click Get Started
        When I select the "sprocket" div

    Scenario: Refresh Log
    	When I click on the "Log" option in the "settingsSidebar"
    	And I should see "Server Log"
    	And I should see "UI Log"
    	And I click the "Refresh" link under "utilErrorLog"


    Scenario: Export Full Log
    	When I click the "Export Full Log" link under "utilErrorLog"
    	And I wait 30 seconds
    	Then the log file "hootlog*.logger" should exist
