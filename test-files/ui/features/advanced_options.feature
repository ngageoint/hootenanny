Feature: Advanced Conflation Options

    Scenario: I can add data
        Given I am on Hootenanny
        And I resize the window
        And I click Get Started
        And I press "Add Reference Dataset"
        And I click the "AllDataTypesACucumber" Dataset
        And I press "Add Layer"
        Then I wait 30 "seconds" to see "span.strong" element with text "AllDataTypesACucumber"
        And I press "Add Secondary Dataset"
        And I click the "AllDataTypesBCucumber" Dataset
        And I press "Add Layer"
        Then I wait 30 "seconds" to see "span.strong" element with text "AllDataTypesBCucumber"

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
        And I should see checkbox "Remove Duplicate Ways" checked
        And I should see checkbox "Remove Superfluous Ways" checked
	And I should see checkbox "Handle Roundabouts" unchecked
        And I should see checkbox "Remove Unlikely Intersections" checked
        And I should see checkbox "Split Dual Ways" checked
        And I should see checkbox "Divided Road Marker Implied" checked
	#And I should see checkbox "Remove Duplicate Names" checked
        #And I should see checkbox "Merge Small Ways" checked
        And I should see element "#small_way_merger_threshold" with no value and placeholder "15"
        And I should see checkbox "Remove Empty Areas" checked
        And I should see checkbox "Remove Duplicate Areas" checked
        And I should see checkbox "Remove Elements With No Information" checked
	And I should see checkbox "Split Sharp Road Corners" checked
	And I should see checkbox "Add Missing Type Tags" unchecked
        Then I click on "#hoot_cleaning_options_label"
        And I should see "Rubber Sheeting Options"
        Then I click on "#hoot_rubber_sheeting_options_label"
        And I should see checkbox "Enabled" checked
        And I should see checkbox "Don't Move Reference Layer" checked
        And I should see element "#rubber_sheet_minimum_ties" with no value and placeholder "4"
	And I should see checkbox "Fail When Minimum Tie Points Are Not Found" unchecked
        Then I click on "#hoot_rubber_sheeting_options_label"
        And I should see "General Conflation Options"
        Then I click on "#hoot_general_conflation_options_label"
        And I should see checkbox "Frechet Distance Subline Matcher" unchecked
	And I should see checkbox "Add Review Tags To Individual Features" unchecked
        And I should see element "#unify_optimizer_time_limit" with no value and placeholder "60"
        And I should see checkbox "OGR Split O2S" unchecked
        And I should see checkbox "OGR Esri FCSubtype" checked
        And I should see checkbox "OGR Thematic Structure" checked
        And I should see checkbox "Duplicate Name Remover Case Sensitive" checked
        And I should see element "#element_cache_size_node" with no value and placeholder "10000000"
        And I should see element "#element_cache_size_relation" with no value and placeholder "2000000"
        And I should see element "#element_cache_size_way" with no value and placeholder "2000000"
	And I should see element "#edge_distance_extractor_spacing" with no value and placeholder "5.0"
	And I should see element "#levenshtein_distance_alpha" with no value and placeholder "1.15"
	#And I should see element "#duplicate_node_remover_distance_threshold" with no value and placeholder "0.01"
	And I should see element "#node_matcher_strictness" with no value and placeholder "2"
	And I should see checkbox "Keep Non Word Tokens" unchecked
	And I should see element "#token_min_size" with no value and placeholder "3"
	And I should see checkbox "Tokenize Strings When Translating" checked
	And I should see element "#weighted_metric_distance_extractor_search_radius" with no value and placeholder "-1.0"
        Then I click on "#hoot_general_conflation_options_label"
        And I should see "Road Options"
        Then I click on "#hoot_road_options_label"
        And I should see checkbox "Enabled" checked
        And I should see element "#hoot_road_opt_engine" with no value and placeholder "Network"
        And I should see element "#search_radius_highway" with no value and placeholder "-1"
	And I should see element "#way_merger_min_split_size" with no value and placeholder "5"
        #TODO: test Unifying engine specific options
        # And I should see element "#highway_matcher_heading_delta" with no value and placeholder "5.0"
        # And I should see element "#highway_matcher_max_angle" with no value and placeholder "60"
        Then I click on "#hoot_road_options_label"
        And I should see "Area Options"
        And I click on "#hoot_area_options_label"
        And I should see checkbox "Enabled" checked
        And I click on "#hoot_area_options_label"
        And I should see "Building Options"
        And I click on "#hoot_building_options_label"
        And I should see checkbox "Enabled" checked
        And I should see checkbox "Review If Secondary Layer Building Newer" unchecked
        And I should see element "#building_date_format" with no value and placeholder "yyyy-MM-ddTHH:mm"
        And I should see element "#building_date_tag_key" with no value and placeholder "source:date"
	And I should see checkbox "Review Non 1:1 Building Matches" unchecked
        And I should see checkbox "Keep More Complex Geometry When Auto Merging" checked
        And I click on "#hoot_building_options_label"
        And I should see "POI Options"
        And I click on "#hoot_poi_options_label"
        And I should see checkbox "Enabled" checked
        And I should see checkbox "Ignore Type If Name Present" unchecked
        And I click on "#hoot_poi_options_label"
        And I should see "POI to Polygon Options"
        And I click on "#hoot_poi_poly_options_label"
        And I should see checkbox "Enabled" checked
        And I should see element "#poipolygon_name_score_threshold" with no value and placeholder "0.8"
        And I should see element "#poipolygon_type_score_threshold" with no value and placeholder "0.7"
        And I should see element "#poipolygon_match_distance_threshold" with no value and placeholder "5.0"
        And I should see element "#poipolygon_additional_search_distance" with no value and placeholder "125.0"
        And I should see element "#poipolygon_review_if_matched_types" with no value and placeholder ""
        And I should see checkbox "Disable Same Source Conflation" unchecked
        And I should see checkbox "Disable Same Source Conflation Match Tag Key Prefix Only" checked
        And I should see element "#poipolygon_source_tag_key" with no value and placeholder "source"
        And I should see checkbox "Promote Points With Addresses to POIs" unchecked
        And I should see element "#poipolygon_review_evidence_threshold" with no value and placeholder "1"
	And I should see checkbox "Review Matches Against Multi-Use Buildings" unchecked
        And I click on "#hoot_poi_poly_options_label"
        And I should see "Waterway Options"
        And I click on "#hoot_waterway_options_label"
        And I should see checkbox "Enabled" checked
        And I should see element "#waterway_way_angle_sample_distance" with no value and placeholder "20.0"
        And I should see element "#waterway_way_matcher_heading_delta" with no value and placeholder "150.0"
        And I should see checkbox "Waterway Auto Calculate Search Radius" checked
        And I should see element "#search_radius_waterway" with no value and placeholder "-1"
        And I should see "Waterway Search Radius" not enabled
        And I should see element "#waterway_rubber_sheet_minimum_ties" with no value and placeholder "5"
        And I should see checkbox "Waterway Auto-Search Radius Calculation Rubber Sheet Ref" checked
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
        And I should see element "#hoot_road_opt_engine" with no value and placeholder "Network"
        Then I click on "#hoot_road_options_label"
        And I should see "Area Options"
        And I should see "Building Options"
        And I should see "POI Options"
        And I should see "POI to Polygon Options"
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
    #    And I should see "General Conflation Options"
    #    And I should see "Road Options"
    #    Then I click on "#hoot_road_options_label"
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
    #    Then I wait 30 "seconds" to see "span.strong" element with text "AllDataTypesACucumber"
    #    And I press "Add Secondary Dataset"
    #    And I click the "AllDataTypesBCucumber" Dataset
    #    And I press "Add Layer"
    #    Then I wait 30 "seconds" to see "span.strong" element with text "AllDataTypesBCucumber"

    #Scenario: I can set up standard conflation parameters
    #    Then I should see "Conflate"
    #    And I press "Conflate"
    #    And I select the "Last Advanced Settings" option in "#containerofConfType"
    #    Then I click the "►" link
    #    And I should see "Advanced Conflation Options"
    #    And I click on "#hoot_poi_options_label"
    #    And I should see checkbox "Enabled" checked
    #    And I click on "#hoot_poi_options_label"
