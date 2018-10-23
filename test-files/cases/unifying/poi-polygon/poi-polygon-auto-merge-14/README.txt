A bug in the address parsing originally caused one of the POI way nodes to match the polygon on address and merge into it. Even with auto-merge
enabled, neither of the POI way nodes should merge into the polygon since both generated reviews against the polygon and did not match 
with it.


