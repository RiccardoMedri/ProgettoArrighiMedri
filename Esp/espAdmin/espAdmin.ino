//questo script viene eseguito solamente nei nodi admin ai quali spettano privilegi di gestione
//istanzia un webserver su cui vengono impostate diverse route
//tramite queste route si possono gestire aggiunta/rimozione tessera, limite accessi, storico ingressi
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <WifiClient.h>
#include "setupConnection.h"

// #define GREEN_LED 8
// #define RED_LED 9

const char* ssid     = "";
const char* password = "";

const char index_html[] PROGMEM = R"rawliteral(
    <!DOCTYPE HTML><html><head>
    <title>ESP Input Form</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    </head><body>
        <ul style = "list-style-type: none;">
            <li>
                <p>Inserire i dati per una nuova tessera</p>
                <form action="/addCard">
                    Nome: <input type="text" name="name">
                    Cognome: <input type="text" name="surname">
                    Ruolo: <input type="text" name="ruolo">
                    <input type="submit" value="Submit">
                </form>
            </li>
            <li style = "padding-top: 3%;">
                <p>Limitare il numero di accessi</p>
                <form action="/limitAccess">
                    <select name = "number" id = "number">
                    </select>
                    <input type="submit" value="Submit">
                </form>
            </li>
            <li style = "padding-top: 3%;">
                <p>Cancellare una tessera</p>
                <form action="/deleteCard">
                    <input type="submit" value="Submit">
                </form>
            </li>
            <li style = "padding-top: 3%;">
              <p>Visualizzare le timbrature di una tessera</p>
              <form action="/getData">
                  Nome: <input type="text" name="name">
                  Cognome: <input type="text" name="surname">
                  <input type="submit" value="Submit">
              </form>
            </li>
        </ul>
    <br>
    </body>
    <script>
        let res = "";
        for(let i = 0; i < 101; ++i) {
            res += "<option>" + i + "</option>";
        }
        document.getElementById("number").innerHTML = res;
    </script>
</html>)rawliteral";

ESP8266WebServer server(80);
WiFiClient wifiClient;
String urlAddCard = "http://192.168.1.22:3000/tessera/nuovaTessera/";
String urlAccess = "http://192.168.1.22:3000/tessera/limitaAccessi/";
String urlDeleteCard = "http://192.168.1.22:3000/tessera/deleteCard/";
String urlGetData = "http://192.168.1.22:3000/timbrature/";
String macAddress = WiFi.macAddress();

void setup() {
  Serial.begin(9600);
  // pinMode(GREEN_LED, OUTPUT);
  // pinMode(RED_LED, OUTPUT);
  delay(10);
  setupConnection(ssid, password);
  setupRouting();
}

void loop() {
  server.handleClient();
}

void setupRouting() {
    server.on("/", handleHome);
    server.on("/addCard", handleAddCard);
    server.on("/limitAccess", handleAccess);
    server.on("/deleteCard", handleDeleteCard);
    server.on("/getData", handleData);
    server.begin();
}

void handleHome(){
    server.send(200, "text/html", index_html);
}

void handleAccess() {
  String limit = server.arg("number");

  // Serial.digitalWrite(GREEN_LED, HIGH);
  // scansione del tag nfc
  // Serial.digitalWrite(GREEN_LED, LOW);
  String uidCard = scanUID();

  String fullUrl = urlAccess + uidCard + "-" + limit + "-" + macAddress;
  String sendResult = httpRequest(fullUrl);

  server.send(200, "text/html", sendResult);
}

void handleAddCard() {
  String name = server.arg("name");
  String surname = server.arg("surname");
  String ruolo = server.arg("ruolo");

  // scansione del tag nfc
  String uidCard = scanUID();

  String fullUrl = urlAddCard + uidCard + "-" + name + "-" + surname + "-" + ruolo + "-" + macAddress;
  String sendResult = httpRequest(fullUrl);

  server.send(200, "text.html", sendResult);
}

void handleDeleteCard() {
  // scansione del tag nfc
  String uidCard = scanUID();

  String fullUrl = urlDeleteCard + uidCard + "-" + macAddress;
  String sendResult = httpRequest(fullUrl);

  server.send(200, "text/html", sendResult);
}

void handleData() {
  String name = server.arg("name");
  String surname = server.arg("surname");
  String fullUrl = urlGetData + name + "-" + surname + "-" + macAddress;

  String result = httpRequest(fullUrl);
  String script = "<script> let arr = " + result + "; let result = ''; for(let i = 0; i < arr.length; ++i) { result += '<tr>' + '<td>' + arr[i]['data'] + '</td>' + '<td>' + arr[i]['orario'] + '</td>' + '</tr>'} document.getElementById('data').innerHTML += result; </script>";
  String index = "<p>Visualizzare le timbrature di una tessera</p> <form action='/getData'> Nome: <input type='text' name='name'> Cognome: <input type='text' name='surname'> <input type='submit' value='Submit'></form> <table style = 'padding-top: 3%;' id = 'data'><dr><th>Data</th><th>Orario</th></dr></table>" + script;

  server.send(200, "text/html", index);
}

String httpRequest(String fullUrl) {
  HTTPClient http;
  http.begin(wifiClient, fullUrl.c_str());
  int httpResponseCode = http.GET();

  if(httpResponseCode > 0) {
    String response = "";
    switch (httpResponseCode) {
      case 200:
        response = http.getString();
        break;
      case 404:
        response = "risorsa non disponibile";
        break;
      default:
        response = "codice di errore: " + String(httpResponseCode);
        break;
    }

    http.end();
    return response;
  }

  http.end();
  return "Response code < 0";
}

String scanUID() {
  Serial.println("GREEN LED IS ON");
  String uidCard = "";
  while(uidCard == "") {
    uidCard = Serial.readString();
  }
  Serial.println("GREEN LED IS OFF");

  return uidCard;
}