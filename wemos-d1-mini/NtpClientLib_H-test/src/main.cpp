#include <TimeLib.h>
#include <NtpClientLib.h>
#include <ESP8266WiFi.h>

#ifndef WIFI_CONFIG_H
/**
 * Update wifi credentials
 * */
#define YOUR_WIFI_SSID "<ssid>"
#define YOUR_WIFI_PASSWD "<password>"
#endif // !WIFI_CONFIG_H

// #define ONBOARDLED 2 // Built in LED on ESP-12/ESP-07
#define SHOW_TIME_PERIOD 5000
#define NTP_TIMEOUT 1500

int8_t timeZone = 4;
int8_t minutesTimeZone = 30;
const PROGMEM char *ntpServer = "asia.pool.ntp.org";
bool wifiFirstConnected = false;

void onSTAConnected (WiFiEventStationModeConnected ipInfo) {
    Serial.printf ("Connected to %s\r\n", ipInfo.ssid.c_str ());
}


// Start NTP only after IP network is connected
void onSTAGotIP (WiFiEventStationModeGotIP ipInfo) {
    Serial.printf ("Got IP: %s\r\n", ipInfo.ip.toString ().c_str ());
    Serial.printf ("Connected: %s\r\n", WiFi.status () == WL_CONNECTED ? "yes" : "no");
    // digitalWrite (ONBOARDLED, LOW); // Turn on LED
    wifiFirstConnected = true;
}

// Manage network disconnection
void onSTADisconnected (WiFiEventStationModeDisconnected event_info) {
    Serial.printf ("Disconnected from SSID: %s\n", event_info.ssid.c_str ());
    Serial.printf ("Reason: %d\n", event_info.reason);
    // digitalWrite (ONBOARDLED, HIGH); // Turn off LED
    //NTP.stop(); // NTP sync can be disabled to avoid sync errors
    WiFi.reconnect ();
}

// void processSyncEvent (NTPSyncEvent_t ntpEvent) {
//     if (ntpEvent < 0) {
//         Serial.printf ("Time Sync error: %d\n", ntpEvent);
//         if (ntpEvent == noResponse)
//             Serial.println ("NTP server not reachable");
//         else if (ntpEvent == invalidAddress)
//             Serial.println ("Invalid NTP server address");
//         else if (ntpEvent == errorSending)
//             Serial.println ("Error sending request");
//         else if (ntpEvent == responseError)
//             Serial.println ("NTP response error");
//     } else {
//         if (ntpEvent == timeSyncd) {
//             Serial.print ("Got NTP time: ");
//             Serial.println (NTP.getTimeDateString (NTP.getLastNTPSync ()));
//         }
//     }
// }

// boolean syncEventTriggered = false; // True if a time even has been triggered
// NTPSyncEvent_t ntpEvent; // Last triggered event

void setup () {
    static WiFiEventHandler e1, e2, e3;

    Serial.begin (115200);
    Serial.println ();
    WiFi.mode (WIFI_STA);
    WiFi.begin (YOUR_WIFI_SSID, YOUR_WIFI_PASSWD);

    e1 = WiFi.onStationModeGotIP (onSTAGotIP);// As soon WiFi is connected, start NTP Client
    e2 = WiFi.onStationModeDisconnected (onSTADisconnected);
    e3 = WiFi.onStationModeConnected (onSTAConnected);
}

void loop () {
    static int i = 0;
    static int last = 0;

    if (wifiFirstConnected) {
        wifiFirstConnected = false;
        NTP.setInterval (63);
        NTP.setNTPTimeout (NTP_TIMEOUT);
        NTP.begin (ntpServer, timeZone, true, minutesTimeZone);
    }

    // if (syncEventTriggered) {
    //     processSyncEvent (ntpEvent);
    //     syncEventTriggered = false;
    // }

    if ((millis () - last) > SHOW_TIME_PERIOD) {
        //Serial.println(millis() - last);
        last = millis ();
        Serial.print (i); Serial.print (" ");
        Serial.println (NTP.getTimeDateString ()); 
        i++;
    }
    delay (0);
}