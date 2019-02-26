package hoot.services.controllers.conflation;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class ConflationConfs {
	private static Map<String, String> conflationConfs = new HashMap<String, String>(){{
		put("Differential", "Diff");
		put("Cookie Cutter & Horizontal", "Horizontal");
		put("Attribute", "Attribute");
		put("Network", "Network");
		put("Reference", "Reference");
	}};

	public static String get(String key) {
		return conflationConfs.get(key);
	}
}
