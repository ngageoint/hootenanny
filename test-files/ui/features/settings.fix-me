@javascript
Feature: Hootenanny Settings Test
	As a software tester
	I want test the manage tab
	So I can ensure that it is working properly

	Scenario: Starting Hootenanny
		Given I am on the Hootenanny site
		Then I click Get Started

	Scenario: Validating Hootenanny Version
		When I click the "Hootenanny" link
		And I should see "Hootenanny Core - Version:"
		And I should see "Hootenanny Web Services - Version:"
		And I should see "Hootenanny iD "
		#When I click the Download User Guide button
		#Then I should download the User Guide
		Then I select the "x" div

	Scenario: Opening Management Tab
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
		And I should see "Error Log"
		And I should see "UI Log"
		Then I click on the "Reports" option in the "settingsSidebar"
		Then I click on the "About" option in the "settingsSidebar"
		And I should see "Hootenanny Core - Version:"
		And I should see "Hootenanny Web Services - Version:"
		And I should see "Hootenanny iD " 
		Then I select the "sprocket" div

