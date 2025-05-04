# TODO

 - ASIO SDK only allows for one active device at a time, the api should be changed so that only
    one device is held active in memory at a time, when the hostapi's are initialized all devices
    should be queried for their name, afterwards only one should be held in memory  (remove the
    array of _asio_devices*)
