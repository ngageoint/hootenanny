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

	Scenario: I can view default Reference Advanced Options
		When I select the "Reference" option in "#containerofConfType"
		Then I click the "►" link
		And I should see "Advanced Conflation Options"
		Then I should see "Cleaning Options"
		Then I click on "#hoot_cleaning_options_label"
		And I should see checkbox "Enabled" checked
		And I should see checkbox "Select All" unchecked
		And I should see checkbox "Remove Duplicate Way" checked
		And I should see checkbox "Remove Superfluous Way" checked
		And I should see checkbox "DualWay Splitter" checked
		And I should see checkbox "Implied Divided Marker" checked
		And I should see checkbox "Small Way Merger" checked
		And I should see element "#small_way_merger_threshold" with no value and placeholder "15"
		And I should see checkbox "Remove Empty Areas" checked
		And I should see checkbox "Remove Duplicate Area" checked
		And I should see checkbox "Remove No Information Element" checked
		Then I click on "#hoot_cleaning_options_label"
		And I should see "Rubber Sheeting Options"
		Then I click on "#hoot_rubber_sheeting_options_label"
		And I should see checkbox "Enabled" unchecked
		And I should see checkbox "Rubber Sheet Ref" unchecked
		And I should see element "#rubber_sheet_minimum_ties" with no value and placeholder "10"
		Then I click on "#hoot_rubber_sheeting_options_label"
		And I should see "General Conflation Options"
		Then I click on "#hoot_general_conflation_options_label"
		And I should see element "#unify_optimizer_time_limit" with no value and placeholder "30"
		And I should see checkbox "OGR Split O2S" unchecked
		And I should see checkbox "OGR TDS Add FCSubtype" checked
		And I should see checkbox "OGR TDS Structure" checked
		And I should see checkbox "Duplicate Name Case Sensitive" checked
		Then I click on "#hoot_general_conflation_options_label"
		And I should see "Road Options"
		Then I click on "#hoot_road_options_label"
		And I should see checkbox "Enabled" checked
		And I should see element "#hoot_road_opt_engine" with no value and placeholder "Unify"
		And I should see element "#search_radius_highway" with no value and placeholder "-1"
		And I should see element "#highway_matcher_heading_delta" with no value and placeholder "5.0"
		And I should see element "#highway_matcher_max_angle" with no value and placeholder "60"
		And I should see element "#way_merger_min_split_size" with no value and placeholder "5"
		Then I click on "#hoot_road_options_label"
		And I should see "Building Options"
		And I click on "#hoot_building_options_label"
		And I should see checkbox "Enabled" checked
		And I click on "#hoot_building_options_label"
		And I should see "POI Options"
		And I click on "#hoot_poi_options_label"
		And I should see checkbox "Enabled" checked
		And I should see element "#hoot_poi_options_list" with no value and placeholder "Unifying"
		And I click on "#hoot_poi_options_label"
		And I should see "Waterway Options"
		And I click on "#hoot_waterway_options_label"
		And I should see checkbox "Enabled" checked
		And I should see element "#waterway_way_angle_sample_distance" with no value and placeholder "20.0"
		And I should see element "#waterway_way_matcher_heading_delta" with no value and placeholder "150.0"
		And I should see checkbox "Waterway Auto Calc Search Radius" checked
		And I should see element "#search_radius_waterway" with no value and placeholder "-1"
		And I should see element "#waterway_rubber_sheet_minimum_ties" with no value and placeholder "5"
		And I should see checkbox "Waterway Rubber Sheet Ref" checked
		And I click on "#hoot_waterway_options_label"
	 	Then I click the "x" icon
	 	And I accept the alert
	 	And I should see "►"
	 	And I should see element ".ConfType" with value "Reference"

	Scenario: I can view default Average Advanced Options 
		When I select the "Average" option in "#containerofConfType"
		Then I click the "►" link
		And I should see "Advanced Conflation Options"
		And I should see "Cleaning Options"
		And I should see "Rubber Sheeting Options"
		And I should see "General Conflation Options"
		And I should see "Road Options"
		Then I click on "#hoot_road_options_label"
		And I should see checkbox "Enabled" checked
		And I should see element "#hoot_road_opt_engine" with no value and placeholder "Greedy"
		And I should see "Building Options"
		And I should see "POI Options"
		And I should see "Waterway Options"
	 	Then I press "big.loud-red" span with text "Cancel"
	 	And I should see "►"
	 	And I should see element ".ConfType" with value "Average"

	# Scenario: I can view and change Horizontal Advanced Options
	#  	When I select the "Cookie Cutter & Horizontal" option in "#containerofConfType"
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
	#  	Then I press "big.loud" span with text "Apply"
	# 	And I should see "►"
	# 	And I should see element ".ConfType" with value "Advanced Conflation"