This application listens for beaconSettings messages coming from the mocap. When it gets one, it calls the 
beagletriangulater's triangleCommand service and tells it the details about the beacons. It commands it to set up its 
localization based on thost beacon arrangements. 