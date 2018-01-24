constants {
    CoAP_Resource_Location = "datagram://localhost:5683",
    MQTT_Broker_Location = "socket://iot.eclipse.org:1883"
}

type requestType: string { .id? : string } | void { .id? : string }

interface resourceInterface {
    OneWay: 
        ledState( requestType )
    RequestResponse: 
        getTmp( requestType )( string ),
        core( void )( string )
}