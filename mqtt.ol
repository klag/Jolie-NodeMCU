include "interface.iol"

outputPort Resource {
    Location: MQTT_Broker_Location // in output we directly speak with the broker
    Protocol: mqtt {
        .debug = false;
        .osc.ledState << {
            .format = "raw", // binary format, json or xml
            .alias = "%!{id}/LED_STATE", // direct mapping with the topic
            .QoS = 1 // quality of service: 0 (AT MOST ONCE),1 (AT LEAST ONCE) ,2 (EXACTLY ONCE)
        };
        .osc.getTmp << {
            .format = "raw",
            .alias = "%!{id}/GET_TEMPERATURE", // request topic (sensor should be subscribed!)
            .aliasResponse = "GET_TEMPERATURE_RESPONSE", // response topic, could be omitted
            .QoS = 1
        }
    }
    Interfaces: resourceInterface
}