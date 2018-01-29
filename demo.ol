/* 

CHECK LIST
1. NodeMCU to USB port
2. MAC WiFi connected to SPZ-Mobile
3. NodeMCU IP address, if changed should be changed in interface.iol too

*/

include "console.iol"

/* (different) DEPLOYMENTS */

include "coap.ol"
//include "mqtt.ol"

/* (same) BEHAVIOUR */

inputPort HTTP {
    Location: "socket://localhost:8000"
    Protocol: http {
        .format = "json"
    }
    RequestResponse: getTemp
}

main {

    /* CoAP specific discovery service */
    // println@Console( "Resources available @ CoAP actuator are:\n" )( ) | {
    //     core@Resource( { .id = "42" } )( response );
    //     println@Console( response )()
    // };

    ledState@Resource( "ON" { .id = "42" } );
    getTmp@Resource( { .id = "42" } )( response );
    getTemp()( response );
    println@Console( "\nSensor n.42 forwarded temperature " + response + " C" )();
    ledState@Resource( "OFF" { .id = "42" } )

}