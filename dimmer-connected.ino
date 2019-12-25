#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "secrets.h"

ESP8266WebServer webServer(80);

bool onOff = false;
unsigned int dimValue = 0;

const char *argDimValue = "dim_value";
const char *argOnOff = "on_off";

const char *dataIndex = "<!DOCTYPE html><html lang=\"en\"><head> <meta charset=\"UTF-8\"> <meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\"> <link rel=\"manifest\" href=\"manifest.json\"> <style>@import url('https://fonts.googleapis.com/css?family=Open+Sans&display=swap'); body, html{padding: 0; margin: 0;}body{font-family: 'Open Sans', sans-serif; user-select: none;}header{width: 100%; height: 50px; line-height: 50px; color: white; font-size: 20px; text-align: center; background-color: #6450ff;}#text{margin: 20px 20px 0 20px; padding: 20px; background-color: #f6f6f6;}#image{margin: 20px 20px 0 20px; text-align: center;}#image img{width: 100%; height: auto;}#bottom{position: fixed; bottom: 0px; left: 0; right: 0;}#bottom-inner{width: 100%;}#buttons{margin: 0 20px 20px 20px; padding: 20px; background-color: #f6f6f6;}#buttons button{width: 45%; padding: 20px 0;}#buttons button.btn-active{font-weight: bold;}#buttons #spacer{display: inline-block; width: 10%;}#slider{margin: 0 20px 20px 20px; background-color: #f6f6f6; height: 50px;}#slider-fill{background-color: #6450ff; height: 50px; width: 100%;}</style> <meta name=\"mobile-web-app-capable\" content=\"yes\"> <meta name=\"apple-mobile-web-app-capable\" content=\"yes\"> <meta name=\"application-name\" content=\"Tuinverlichting\"> <meta name=\"apple-mobile-web-app-title\" content=\"Tuinverlichting\"> <meta name=\"theme-color\" content=\"#6450ff\"> <meta name=\"msapplication-navbutton-color\" content=\"#6450ff\"> <meta name=\"apple-mobile-web-app-status-bar-style\" content=\"black-translucent\"> <meta name=\"msapplication-starturl\" content=\"/\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1, shrink-to-fit=no\"> <link rel=\"icon\" type=\"image/png\" sizes=\"72x72\" href=\"https://tobiass.nl/tuin-app/icons/icon-72x72.png\"> <link rel=\"apple-touch-icon\" type=\"image/png\" sizes=\"72x72\" href=\"https://tobiass.nl/tuin-app/icons/icon-72x72.png\"> <link rel=\"icon\" type=\"image/png\" sizes=\"96x96\" href=\"https://tobiass.nl/tuin-app/icons/icon-96x96.png\"> <link rel=\"apple-touch-icon\" type=\"image/png\" sizes=\"96x96\" href=\"https://tobiass.nl/tuin-app/icons/icon-96x96.png\"> <link rel=\"icon\" type=\"image/png\" sizes=\"128x128\" href=\"https://tobiass.nl/tuin-app/icons/icon-128x128.png\"> <link rel=\"apple-touch-icon\" type=\"image/png\" sizes=\"128x128\" href=\"https://tobiass.nl/tuin-app/icons/icon-128x128.png\"> <link rel=\"icon\" type=\"image/png\" sizes=\"144x144\" href=\"https://tobiass.nl/tuin-app/icons/icon-144x144.png\"> <link rel=\"apple-touch-icon\" type=\"image/png\" sizes=\"144x144\" href=\"https://tobiass.nl/tuin-app/icons/icon-144x144.png\"> <link rel=\"icon\" type=\"image/png\" sizes=\"152x152\" href=\"https://tobiass.nl/tuin-app/icons/icon-152x152.png\"> <link rel=\"apple-touch-icon\" type=\"image/png\" sizes=\"152x152\" href=\"https://tobiass.nl/tuin-app/icons/icon-152x152.png\"> <link rel=\"icon\" type=\"image/png\" sizes=\"192x192\" href=\"https://tobiass.nl/tuin-app/icons/icon-192x192.png\"> <link rel=\"apple-touch-icon\" type=\"image/png\" sizes=\"192x192\" href=\"https://tobiass.nl/tuin-app/icons/icon-192x192.png\"> <link rel=\"icon\" type=\"image/png\" sizes=\"384x384\" href=\"https://tobiass.nl/tuin-app/icons/icon-384x384.png\"> <link rel=\"apple-touch-icon\" type=\"image/png\" sizes=\"384x384\" href=\"https://tobiass.nl/tuin-app/icons/icon-384x384.png\"> <link rel=\"icon\" type=\"image/png\" sizes=\"512x512\" href=\"https://tobiass.nl/tuin-app/icons/icon-512x512.png\"> <link rel=\"apple-touch-icon\" type=\"image/png\" sizes=\"512x512\" href=\"https://tobiass.nl/tuin-app/icons/icon-512x512.png\"> <title>Tuinverlichting</title> <script>window.addEventListener(\"load\",()=>{const e=document.querySelector(\"#slider\"),t=document.querySelector(\"#slider-fill\"),n=document.querySelector(\"#btn-on\"),i=document.querySelector(\"#btn-off\");let o=0,c=!1;function d(){t.style.width=o/1023*100+\"%\",c?(n.className=\"btn-active\",i.className=\"\"):(i.className=\"btn-active\",n.className=\"\")}function a(t){const n=e.getBoundingClientRect();let i=t.clientX-n.x;i<0&&(i=0),i>n.width&&(i=n.width),o=Math.floor(i/n.width*1023),d()}fetch(\"/api/get\").then(e=>e.json()).then(t=>{o=t.dim_value,c=t.on_off,d();let l=!1;e.addEventListener(\"pointerdown\",e=>{l=!0,a(e)}),e.addEventListener(\"pointermove\",e=>{l&&a(e)}),e.addEventListener(\"pointercancel\",()=>{l=!1}),e.addEventListener(\"pointerup\",()=>{l=!1,fetch(\"/api/set?dim_value=\"+o)}),n.addEventListener(\"click\",()=>{fetch(\"/api/set?on_off=true\"),c=!0,d()}),i.addEventListener(\"click\",()=>{fetch(\"/api/set?on_off=false\"),c=!1,d()})})}); </script></head><body> <header> Tuinverlichting </header> <div id=\"text\"> Bedien hieronder de tuinverlichting. Met de schuif is de helderheid in te stellen, met de knoppen is het geheel in een keer aan of uit te zetten. </div><div id=\"image\"> <img src=\"https://tobiass.nl/tuin-app/huisje-scaled.jpg\"> </div><div id=\"bottom\"> <div id=\"bottom-inner\"> <div id=\"buttons\"> <button id=\"btn-off\">Licht uit</button><div id=\"spacer\"></div><button id=\"btn-on\">Licht aan</button> </div><div id=\"slider\"> <div id=\"slider-fill\"></div></div></div></div></body></html>";
const char *dataManifest = "{\"theme_color\":\"#6450ff\",\"display\":\"standalone\",\"Scope\":\"/\",\"start_url\":\"/\",\"icons\":[{\"src\":\"https://tobiass.nl/tuin-app/icons/icon-72x72.png\",\"sizes\":\"72x72\",\"type\":\"image/png\"},{\"src\":\"https://tobiass.nl/tuin-app/icons/icon-96x96.png\",\"sizes\":\"96x96\",\"type\":\"image/png\"},{\"src\":\"https://tobiass.nl/tuin-app/icons/icon-128x128.png\",\"sizes\":\"128x128\",\"type\":\"image/png\"},{\"src\":\"https://tobiass.nl/tuin-app/icons/icon-144x144.png\",\"sizes\":\"144x144\",\"type\":\"image/png\"},{\"src\":\"https://tobiass.nl/tuin-app/icons/icon-152x152.png\",\"sizes\":\"152x152\",\"type\":\"image/png\"},{\"src\":\"https://tobiass.nl/tuin-app/icons/icon-192x192.png\",\"sizes\":\"192x192\",\"type\":\"image/png\"},{\"src\":\"https://tobiass.nl/tuin-app/icons/icon-384x384.png\",\"sizes\":\"384x384\",\"type\":\"image/png\"},{\"src\":\"https://tobiass.nl/tuin-app/icons/icon-512x512.png\",\"sizes\":\"512x512\",\"type\":\"image/png\"}],\"splash_pages\":null}";

void setOutput() {
  if (onOff) {
    analogWrite(D0, dimValue);
  } else {
    analogWrite(D0, 0);
  }
}

void serveNotFound() {
  webServer.send(404, "text/plain", "404");
}

void serveIndex() {
  webServer.send(200, "text/html", dataIndex);
}

void serveManifest() {
  webServer.send(200, "application/json", dataManifest);
}

void handleSet() {
  if (!webServer.hasArg(argOnOff) && !webServer.hasArg(argDimValue)) {
    webServer.send(404, "text/plain", "404");
    return;
  }

  if (webServer.hasArg(argOnOff)) {
    String s1 = webServer.arg(argOnOff);
  
    if (s1.equals("true")) {
      onOff = true;
    } else if (s1.equals("false")) {
      onOff = false;
    }
  }

  if (webServer.hasArg(argDimValue)) {
    String s2 = webServer.arg(argDimValue);
    
    int i = s2.toInt();
  
    if (i >= 0 || i <= 1023) {
      dimValue = i;
    }
  }

  setOutput();

  handleGet();
}

void handleGet() {
  webServer.send(200, "application/json", "{\"on_off\": " + (onOff ? String("true") : String("false"))+ ", \"dim_value\": " + String(dimValue) + "}");
}

void setup() {
  pinMode(D0, OUTPUT);
  setOutput();
  
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(wifiSsid, wifiPassword);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  webServer.onNotFound(serveNotFound);
  webServer.on("/", HTTP_GET, serveIndex);
  webServer.on("/manifest.json", HTTP_GET, serveManifest);
  webServer.on("/api/set", HTTP_GET, handleSet);
  webServer.on("/api/get", HTTP_GET, handleGet);
  webServer.begin();
  
}

void loop() {
  webServer.handleClient();
}

