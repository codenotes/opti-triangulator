This application listens for beaconSettings messages coming from the mocap. When it gets one, it calls the 
beagletriangulater's triangleCommand service and tells it the details about the beacons. It commands it to set up its 
localization based on thost beacon arrangements. 

NOTE: get asserition error on line 52 dgbdel.cpp when using 2015 platform.  2013 is ok.  Not sure what issue might be, so dropped back down. 
MAKE SURE YOU ARE LINKING AGAINST THE RIGHT PLATFORM ROSDLL, 2013 or 2015.
