

#include <WiFiS3.h>

#include "DHT.h"
//#include "arduino_secrets.h" 
char ssid[] = "TP-Link_E5F8";        // your network SSID (name)
char pass[] = "51354916";    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;     // the WiFi radio's status


#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22 (AM2302), AM2321



// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);


void setup() 
{
    // put your setup code here, to run once:
    Serial.begin(9600);
    while (!Serial) 
    {
        ; // wait for serial port to connect. Needed for native USB port only
    }
    // check for the WiFi module:
    if (WiFi.status() == WL_NO_MODULE) 
    {
        Serial.println("Communication with WiFi module failed!");
        // don't continue
        while (true);
    }

    String fv = WiFi.firmwareVersion();
    if (fv < WIFI_FIRMWARE_LATEST_VERSION) 
    {
        Serial.println("Please upgrade the firmware");
    }

  // attempt to connect to WiFi network:
    while (status != WL_CONNECTED) 
    {
        Serial.print("Attempting to connect to WPA SSID: ");
        Serial.println(ssid);
        // Connect to WPA/WPA2 network:
        status = WiFi.begin(ssid, pass);

        // wait 10 seconds for connection:
        delay(10000);
    }

    // you're connected now, so print out the data:
    Serial.print("You're connected to the network");
    printCurrentNet();
    printWifiData();

    dht.begin();
//  delay(2000);

}

void loop() {
    // check the network connection once every 10 seconds:
    delay(1000);
    printCurrentNet();

    // read the input on analog pin 0:
    int sensorValue = analogRead(A0);

    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) 
    {
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
    }

    // Compute heat index in Fahrenheit (the default)
    float hif = dht.computeHeatIndex(f, h);
    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);


    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.print(F("°C "));


    // print out the value you read:
    Serial.print("Soil Humidity:");
    Serial.println(sensorValue);
    delay(50);        // delay in between reads for stability


}


void printWifiData() 
{
    // print your board's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    
    Serial.println(ip);

    // print your MAC address:
    byte mac[6];
    WiFi.macAddress(mac);
    Serial.print("MAC address: ");
    printMacAddress(mac);
}

void printCurrentNet() {
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print the MAC address of the router you're attached to:
    byte bssid[6];
    WiFi.BSSID(bssid);
    Serial.print("BSSID: ");
    printMacAddress(bssid);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.println(rssi);

    // print the encryption type:
    byte encryption = WiFi.encryptionType();
    Serial.print("Encryption Type:");
    Serial.println(encryption, HEX);
    Serial.println();
}

void printMacAddress(byte mac[]) 
{
    for (int i = 0; i < 6; i++) 
    {
        if (i > 0) 
        {
            Serial.print(":");
        }
        if (mac[i] < 16) 
        {
            Serial.print("0");
        }
        Serial.print(mac[i], HEX);
    }
    Serial.println();
}



