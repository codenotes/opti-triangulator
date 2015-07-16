This app reads the mocap and sends out

beaconPublisher which sends out consolodated beaconSettings. Actually, it sends out all 
rigidbodies by name with their location.  It is listened to by beaconLocationClient which
in turn calls the service on BeagleCrossTriangulator which sets up the sensor. 