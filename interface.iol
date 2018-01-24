constants {
    CoAP_Resource_Location = "datagram://172.20.10.8:5683",
    MQTT_Broker_Location = "socket://iot.eclipse.org:1883"
}

type requestType: string { .id? : string } | void { .id? : string }

interface resourceInterface {
    OneWay: 
        ledState( requestType )
    RequestResponse: 
        getTmp( requestType )( string ),
        core( requestType )( string )
}