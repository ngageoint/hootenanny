Feature: Advanced Conflation Options

    Scenario: I can add data
        Given I am on Hootenanny
        And I resize the window
        And I click Get Started
        And I press "Add Reference Dataset"
        And I click the "AllDataTypesACucumber" Dataset
        And I press "Add Layer"
        Then I wait 15 "seconds" to see "span.strong" element with text "AllDataTypesACucumber"
        And I press "Add Secondary Dataset"
        And I click the "AllDataTypesBCucumber" Dataset
        And I press "Add Layer"
        Then I wait 15 "seconds" to see "span.strong" element with text "AllDataTypesBCucumber"

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
        And I should see element "#element_cache_size_node" with no value and placeholder "2000000"
        And I should see element "#element_cache_size_relation" with no value and placeholder "200000"
        And I should see element "#element_cache_size_way" with no value and placeholder "200000"
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
        And I should see "Search Radius Waterway" not enabled
        And I should see element "#waterway_rubber_sheet_minimum_ties" with no value and placeholder "5"
        And I should see checkbox "Waterway Rubber Sheet Ref" checked
        And I click on "#hoot_waterway_options_label"
        Then I click the "x" icon
        And I accept the alert
        And I should see "►"
        And I should see element ".ConfType" with value "Reference"

    Scenario: I can view Horizontal Advanced Options
        When I select the "Cookie Cutter & Horizontal" option in "#containerofConfType"
        Then I click the "►" link
        Then I should see "Advanced Conflation Options"
        And I should see "Cleaning Options"
        And I should see "Rubber Sheeting Options"
        And I should see "General Conflation Options"
        And I should see "Road Options"
        Then I click on "#hoot_road_options_label"
        And I should see checkbox "Enabled" checked
        And I should see element "#hoot_road_opt_engine" with no value and placeholder "Unify"
        Then I click on "#hoot_road_options_label"
        And I should see "Building Options"
        And I should see "POI Options"
        And I should see "Waterway Options"
        And I should see "Cookie Cutter & Horizontal Options"
        Then I click on "#horizontal_conflation_options_label"
        And I should see element "#horizontal_cookie_cutter_alpha_shape_buffer" with no value and placeholder "0"
        Then I fill "#horizontal_cookie_cutter_alpha_shape_buffer" with "5"
        Then I click on "#horizontal_conflation_options_label"
        Then I press "big.loud-red" span with text "Cancel"
        And I should see "►"
        And I should see element ".ConfType" with value "Cookie Cutter & Horizontal"

    # TODO: fix the next three scenarios as part of ngageoint/hootenanny-ui#764

    #Scenario: I can view and change Average Advanced Options
    #    When I select the "Average" option in "#containerofConfType"
    #    Then I click the "►" link
    #    And I should see "Advanced Conflation Options"
    #    And I should see "Cleaning Options"
    #    And I click on "#hoot_cleaning_options_label"
    #    Then I should see checkbox "Enabled" checked
    #    And I uncheck the "Enabled" checkbox
    #    Then I should see checkbox "Small Way Merger" not enabled
    #    And I should see "Small Way Merger Threshold" not enabled
    #    And I check the "Enabled" checkbox
    #    Then I should see checkbox "Small Way Merger" enabled
    #    And I should see "Small Way Merger Threshold" enabled
    #    And I should see checkbox "Select All" unchecked
    #    And I should see checkbox "Small Way Merger" checked
    #    Then I check the "Select All" checkbox
    #    Then I uncheck the "Select All" checkbox
    #    And I should see checkbox "Select All" unchecked
    #    And I should see checkbox "Small Way Merger" unchecked
    #    Then I check the "Select All" checkbox
    #    And I should see checkbox "Small Way Merger" checked
    #    Then I uncheck the "Small Way Merger" checkbox
    #    And I wait 2 "seconds" to not see "Small Way Merger Threshold"
    #    Then I check the "Small Way Merger" checkbox
    #    And I wait 2 "seconds" to see "Small Way Merger Threshold"
    #    Then I click on "#hoot_cleaning_options_label"
    #    Then I should see "Rubber Sheeting Options"
    #    And I click on "#hoot_rubber_sheeting_options_label"
    #    And I should see checkbox "Enabled" unchecked
    #    And I should see "Rubber Sheet Ref" not enabled
    #    And I should see "Rubber Sheet Minimum Ties" not enabled
    #    And I check the "Enabled" checkbox
    #    And I should see "Rubber Sheet Ref" enabled
    #    And I should see "Rubber Sheet Minimum Ties" enabled
    #    And I uncheck the "Enabled" checkbox
    #    Then I click on "#hoot_rubber_sheeting_options_label"
    #    And I should see "General Conflation Options"
    #    And I should see "Road Options"
    #    Then I click on "#hoot_road_options_label"
    #    And I should see element "#hoot_road_opt_engine" with no value and placeholder "Greedy"
    #    And I should not see "Search Radius Highway"
    #    Then I select the "Unify" option labelled "Engines"
    #    And I should see checkbox "Enabled" checked
    #    And I uncheck the "Enabled" checkbox
    #    And I should see "Engines" combobox not enabled
    #    And I should see "Search Radius Highway" not enabled
    #    And I should see "Highway Matcher Max Angle" not enabled
    #    And I check the "Enabled" checkbox
    #    And I should see "Engines" combobox enabled
    #    And I should see "Search Radius Highway" enabled
    #    And I should see "Highway Matcher Max Angle" enabled
    #    And I uncheck the "Enabled" checkbox
    #    Then I click on "#hoot_road_options_label"
    #    And I should see "Building Options"
    #    And I click on "#hoot_building_options_label"
    #    And I uncheck the "Enabled" checkbox
    #    And I click on "#hoot_building_options_label"
    #    And I should see "POI Options"
    #    And I click on "#hoot_poi_options_label"
    #    Then I should see checkbox "Enabled" checked
    #    And I uncheck the "Enabled" checkbox
    #    And I should see "POI Options" combobox not enabled
    #    Then I check the "Enabled" checkbox
    #    And I should see "POI Options" combobox enabled
    #    Then I select the "Places" option in the "Unifying" combobox
    #    And I click on "#hoot_poi_options_label"
    #    And I should see "Waterway Options"
    #    And I click on "#hoot_waterway_options_label"
    #    And I should see checkbox "Enabled" checked
    #    And I should see "Waterway Angle Sample Distance" enabled
    #    Then I uncheck the "Enabled" checkbox
    #    And I should see "Waterway Angle Sample Distance" not enabled
    #    And I should see "Waterway Auto Calc Search Radius" not enabled
    #    Then I check the "Enabled" checkbox
    #    And I should see "Waterway Angle Sample Distance" enabled
    #    And I should see "Waterway Auto Calc Search Radius" enabled
    #    Then I uncheck the "Waterway Auto Calc Search Radius" checkbox
    #    And I should see "Search Radius Waterway" enabled
    #    And I should see "Waterway Rubber Sheet Minimum Ties" not enabled
    #    And I should see "Waterway Rubber Sheet Ref" not enabled
    #    Then I check the "Waterway Auto Calc Search Radius" checkbox
    #    And I should see "Search Radius Waterway" not enabled
    #    And I should see "Waterway Rubber Sheet Minimum Ties" enabled
    #    And I should see "Waterway Rubber Sheet Ref" enabled
    #    Then I uncheck the "Enabled" checkbox
    #    And I click on "#hoot_waterway_options_label"
    #    And I should not see "Cookie Cutter & Horizontal Options"
    #    Then I press "big.loud" span with text "Apply"
    #    And I should see "►"
    #    And I should see element ".ConfType" with value "Custom Average"
    #    And I scroll element into view and press "conflate2"
    #    Then I wait 30 "seconds" to see "Conflating …"
    #    Then I wait 3 "minutes" to see "Merged_AllDataTypes_Advanced"
    #    Then I should see "Complete Review"
    #    And I click the "Complete Review" link
    #    Then I should see "All Reviews Resolved!"
    #    Then I remove the first layer
    #    Then I select the "sprocket" div
    #    And I click the "Merged_AllDataTypes_Advanced" Dataset
    #    And I context click the "Merged_AllDataTypes_Advanced" Dataset
    #    And I click the "Delete (1)" context menu item
    #    Then I accept the alert
    #  # And I wait 30 "seconds" to not see "Merged_AllDataTypes_Advanced"
    #    Then I select the "sprocket" div
    #    And I wait 10 "seconds" to not see "Return to Map"

    #Scenario: Check for previous settings
    #    And I hover over "#map"
    #    And I press "Add Reference Dataset"
    #    And I click the "AllDataTypesACucumber" Dataset
    #    And I press "Add Layer"
    #    Then I wait 15 "seconds" to see "span.strong" element with text "AllDataTypesACucumber"
    #    And I press "Add Secondary Dataset"
    #    And I click the "AllDataTypesBCucumber" Dataset
    #    And I press "Add Layer"
    #    Then I wait 15 "seconds" to see "span.strong" element with text "AllDataTypesBCucumber"

    #Scenario: I can set up standard conflation parameters
    #    Then I should see "Conflate"
    #    And I press "Conflate"
    #    And I select the "Last Advanced Settings" option in "#containerofConfType"
    #    Then I click the "►" link
    #    And I should see "Advanced Conflation Options"
    #    And I click on "#hoot_poi_options_label"
    #    And I should see checkbox "Enabled" checked
    #    And I should see element "#hoot_poi_options_list" with value "Unifying"
    #    And I click on "#hoot_poi_options_label"
