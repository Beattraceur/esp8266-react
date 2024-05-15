#include <RelayStateService.h>

RelayStateService::RelayStateService(AsyncWebServer* server,
                                     SecurityManager* securityManager,
                                     AsyncMqttClient* mqttClient,
                                     RelayMqttSettingsService* relayMqttSettingsService) :
    _httpEndpoint(RelayState::read,
                  RelayState::update,
                  this,
                  server,
                  RELAY_SETTINGS_ENDPOINT_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _mqttPubSub(RelayState::haRead, RelayState::haUpdate, this, mqttClient),
    _webSocket(RelayState::read,
               RelayState::update,
               this,
               server,
               RELAY_SETTINGS_SOCKET_PATH,
               securityManager,
               AuthenticationPredicates::IS_AUTHENTICATED),
    _mqttClient(mqttClient),
    _relayMqttSettingsService(relayMqttSettingsService) {
  // configure led to be output
  pinMode(RELAY_PIN1, OUTPUT);
  pinMode(RELAY_PIN2, OUTPUT);
  pinMode(RELAY_PIN3, OUTPUT);
  pinMode(RELAY_PIN4, OUTPUT);

  // configure MQTT callback
  _mqttClient->onConnect(std::bind(&RelayStateService::registerConfig, this));

  // configure update handler for when the relay settings change
  _relayMqttSettingsService->addUpdateHandler([&](const String& originId) { registerConfig(); }, false);

  // configure settings service update handler to update LED state
  addUpdateHandler([&](const String& originId) { onConfigUpdated(); }, false);
}

void RelayStateService::begin() {
  _state.relay1On = DEFAULT_RELAY_STATE;
  _state.relay2On = DEFAULT_RELAY_STATE;
  _state.relay3On = DEFAULT_RELAY_STATE;
  _state.relay4On = DEFAULT_RELAY_STATE;
  onConfigUpdated();
}

void RelayStateService::onConfigUpdated() {
  digitalWrite(RELAY_PIN1, _state.relay1On ? RELAY_ON : RELAY_OFF);
  digitalWrite(RELAY_PIN2, _state.relay2On ? RELAY_ON : RELAY_OFF);
  digitalWrite(RELAY_PIN3, _state.relay3On ? RELAY_ON : RELAY_OFF);
  digitalWrite(RELAY_PIN4, _state.relay4On ? RELAY_ON : RELAY_OFF);
}

void RelayStateService::registerConfig() {
  if (!_mqttClient->connected()) {
    return;
  }
  String configTopic;
  String subTopic;
  String pubTopic;

  DynamicJsonDocument doc(256);
  _relayMqttSettingsService->read([&](RelayMqttSettings& settings) {
    configTopic = settings.mqttPath + "/config";
    subTopic = settings.mqttPath + "/set";
    pubTopic = settings.mqttPath + "/state";
    doc["~"] = settings.mqttPath;
    doc["name"] = settings.name;
    doc["unique_id"] = settings.uniqueId;
  });
  doc["cmd_t"] = "~/set";
  doc["stat_t"] = "~/state";
  doc["schema"] = "json";
  doc["brightness"] = false;

  String payload;
  serializeJson(doc, payload);
  _mqttClient->publish(configTopic.c_str(), 0, false, payload.c_str());

  _mqttPubSub.configureTopics(pubTopic, subTopic);
}
