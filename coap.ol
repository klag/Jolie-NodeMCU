include "interface.iol"

/* (different) DEPLOYMENT */

outputPort  Resource {
    Location: CoAP_Resource_Location
    Protocol: coap {
        .debug = true;
        .proxy = false; // yes, we could surf over a proxy
        .osc.core << {
            .alias = "/%!{id}/.well-known/core", // Constrained RESTful Environments (CoRE) - charter-ietf-core-02
            .messageCode = "GET", // methods @ https://tools.ietf.org/html/rfc7252#page-46
            .messageType = "CON" // CONfirmable or NON confirmable 
        };
        .osc.ledState << {
            .contentFormat = "text/plain", // content-format option: text/plain, application/xml, application/json, application/link-format (CoRE), application/octet-stream
            .alias = "/%!{id}/LED_STATE", // alias is mapped as HTTP resource URI-Path, note the initial / mandatory
            .messageCode = "PUT",
            .messageType = "CON"
        };
        .osc.getTmp << {
            .contentFormat = "text/plain",
            .alias = "/%!{id}/GET_TEMPERATURE",
            .messageCode = "GET",
            .messageType = "CON"
        }
    }
    Interfaces: resourceInterface
}