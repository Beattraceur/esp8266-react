#ifndef RelayMqttSettingsService_h
#define RelayMqttSettingsService_h

#include <HttpEndpoint.h>
#include <FSPersistence.h>
#include <SettingValue.h>

#define RELAY_BROKER_SETTINGS_FILE "/config/brokerSettings.json"
#define RELAY_BROKER_SETTINGS_PATH "/rest/brokerSettings"

class RelayMqttSettings {
 public:
  String mqttPath;
  String name;
  String uniqueId;

  static void read(RelayMqttSettings& settings, JsonObject& root) {
    root["mqtt_path"] = settings.mqttPath;
    root["name"] = settings.name;
    root["unique_id"] = settings.uniqueId;
  }

  static StateUpdateResult update(JsonObject& root, RelayMqttSettings& settings) {
    settings.mqttPath = root["mqtt_path"] | SettingValue::format("homeassistant/relay/#{unique_id}");
    settings.name = root["name"] | SettingValue::format("relay-#{unique_id}");
    settings.uniqueId = root["unique_id"] | SettingValue::format("relay-#{unique_id}");
    return StateUpdateResult::CHANGED;
  }
};

class RelayMqttSettingsService : public StatefulService<RelayMqttSettings> {
 public:
  RelayMqttSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager);
  void begin();

 private:
  HttpEndpoint<RelayMqttSettings> _httpEndpoint;
  FSPersistence<RelayMqttSettings> _fsPersistence;
};

#endif  // end RelayMqttSettingsService_h
