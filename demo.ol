include "console.iol"

/* (different) DEPLOYMENTS */

//include "coap.ol"
include "mqtt.ol"

/* (same) BEHAVIOUR */

main {

    /* CoAP specific discovery service */
    // println@Console( "Resources available @ CoAP actuator are:\n" )( ) | {
    //     core@Resource( { .id = "42" } )( response );
    //     println@Console( response )()
    // };

    ledState@Resource( "ON" { .id = "42" } );
    
    getTmp@Resource( { .id = "42" } )( response );
    println@Console( "\nSensor n.42 forwarded temperature " + response + " C" )();

    ledState@Resource( "OFF" { .id = "42" } )

}