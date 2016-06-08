Feature: Advanced Conflation Options

	Scenario: I can add data
		Given I am on Hootenanny
	    And I resize the window
	    And I click Get Started
	    And I press "Add Reference Dataset"
	    And I click the "AllDataTypesACucumber" Dataset
	    And I press "Add Layer"
	    Then I wait 30 "seconds" to see "AllDataTypesACucumber"
	    And I press "Add Secondary Dataset"
	    And I click the "AllDataTypesBCucumber" Dataset
	    And I press "Add Layer"
	    Then I wait 30 "seconds" to see "AllDataTypesBCucumber"
	    
	Scenario: I can set up standard conflation parameters
	    Then I should see "Conflate"
	    And I press "Conflate"
	    And I fill "saveAs" input with "Merged_AllDataTypes_Advanced"
	    And I select the "Reference" option in "#containerofConfType"
	    And I should see "►"

	Scenario: I can change Advanced Options
		When I select the "Average" option in "#containerofConfType"
		Then I click the "►" link
		And I should see "Advanced Conflation Options"
		Then I click on "#hoot_cleaning_options_label"
		And I should see "Enabled"

	# Scenario: I can open and close Advanced Options Sidebar
	# 	When I press "confAdvOptsLnk"
	# 	Then I should see "Advanced Conflation Options"
	# 	Then I press "big.loud-red" span with text "Cancel"
	# 	And I should see "►"
	# 	Then I press "confAdvOptsLnk"
	# 	Then I should see "Advanced Conflation Options"
	# 	Then I click the "◄" link
	# 	And I accept the alert
	# 	And I should see "►"
	# 	Then I press "confAdvOptsLnk"
	# 	Then I should see "Advanced Conflation Options"
	# 	Then I click the "x" icon
	# 	And I accept the alert
	# 	And I should see "►"		

	# Scenario: I can view Advanced Options Sidebar with Reference Selected
	# 	When I press "confAdvOptsLnk"
	# 	Then I should see "Advanced Conflation Options"
	# 	And I should see "Cleaning Options"
	# 	And I should see "Rubber Sheeting Options"
	# 	And I should see "General Conflation Options"
	# 	And I should see "Road Options"
	# 	And I should see "Building Options"
	# 	And I should see "POI Options"
	# 	And I should see "Waterway Options"
	# 	Then I press "big.loud-red" span with text "Cancel"
	# 	And I should see "►"

	# Scenario: I can view Advanced Options Sidebar with Horizontal Selected
	# 	When I select the "Cookie Cutter & Horizontal" option in "#containerofConfType"
	# 	Then I click the "►" link
	# 	Then I should see "Advanced Conflation Options"
	# 	And I should see "Cleaning Options"
	# 	And I should see "Rubber Sheeting Options"
	# 	And I should see "General Conflation Options"
	# 	And I should see "Road Options"
	# 	And I should see "Building Options"
	# 	And I should see "POI Options"
	# 	And I should see "Waterway Options"
	# 	And I should see "Cookie Cutter & Horizontal Options"
	# 	Then I click the "◄" link
	# 	And I accept the alert
	# 	And I should see "►"