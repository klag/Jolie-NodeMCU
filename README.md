# Blink NodeMCU with Jolie

Sample code that let you make the NodeMCU open hardware blink with the Jolie Programming Language with a few lines of clean code. The blink could be done with MQTT or CoAP application protocols. This could be an easy starting point for Jolie new users or IoT practitioners.

## Getting Started

You only need to flash NodeMCU with the firmware provided, then run the demo.ol code.

### Prerequisites

1. the Jolie for IoT interpreter to run the Jolie code - instruction and software @ http://cs.unibo.it/projects/jolie/jiot.html
2. the Arduino IDE to flash the NodeMCU - http://arduino.cc, then be sure to install the libraries:
  * MQTT;
  * Simple CoAP library;
  * ESP8266WiFi
  * WifiUDP
3. a text editor, i use Sublime-Text - https://www.sublimetext.com

### Installing

First of all we need to flash the NodeMCU.ino firmware into the NodeMCU, we will use the Arduino IDE.

1. Before flashing modify lines 6-7 to be compatible with your WiFi credentials:

```
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <coap.h>
#include <MQTTClient.h>

const char ssid[] = "your_wifi_essid";
const char pass[] = "your_wifi_password";
...
```

Feel free to change topic names (optional) to suite your topology:

```
char temperature_topic[] = "42/GET_TEMPERATURE"; 
char led_topic[] = "42/LED_STATE";
char discovery_service[] = "42/.well-known/core";
```

End with an example of getting some data out of the system or using it for a little demo

## Running the tests

Explain how to run the automated tests for this system

### Break down into end to end tests

Explain what these tests test and why

```
Give an example
```

### And coding style tests

Explain what these tests test and why

```
Give an example
```

## Deployment

Add additional notes about how to deploy this on a live system

## Built With

* [Dropwizard](http://www.dropwizard.io/1.0.2/docs/) - The web framework used
* [Maven](https://maven.apache.org/) - Dependency Management
* [ROME](https://rometools.github.io/rome/) - Used to generate RSS Feeds

## Contributing

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/your/project/tags). 

## Authors

* **Prof. Maurizio Gabbrielli** - *Theoretical Fundation* - [WebSite](http://cs.unibo.it/~gabbri)
* **Prof. Ivan Lanese** - *Theoretical Fundation* - [WebSite](http://cs.unibo.it/~ilanese)
* **Post Doc. Saverio Giallorenzo** - *Theoretical Fundation* - [WebSite](http://cs.unibo.it/~giallor)
* **PhD Candidate Stefano Pio Zingaro** - *Code and Mainteinance* - [WebSite](http://cs.unibo.it/~stefanopio.zingaro)

See also the list of [contributors](Contributing) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Hat tip to anyone who's code was used
* Inspiration
* etc
