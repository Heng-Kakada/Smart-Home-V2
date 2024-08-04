#ifndef DHT_SENSOR
#define DHT_SENSOR


#include <DHT.h>

class DhtSensor {
  private:

    DHT dht;
    int pin;
    float temp;
    float hum;

    void printOnLcd(float t, float h){
      String temp = "Temperature: " + String(t) + "*C";
      String hum = "Humidity: " + String(h) + " %";
    }

  public:
    DhtSensor(int p, int type) : pin(p), dht(p, type), temp(0), hum(0) {}
    
    void begin() {
      dht.begin();
    }
    void update() {
      temp = dht.readTemperature();
      hum = dht.readHumidity();
    }
    String getData() {
      return "Temp: " + String(temp) + " C, Humidity: " + String(hum) + " %";
    }
    void transmit() {
      update();
      if (isnan(temp) || isnan(hum)){
        temp = 0.0;
        hum = 0.0;
      }
      JsonDocument doc;

      doc["path"] = "dht";
      JsonObject data = doc.createNestedObject("data");
      data["temp"] = temp;
      data["hum"] = hum;

      serializeJson(doc, Serial);
      Serial.println();
    }
};

#endif
