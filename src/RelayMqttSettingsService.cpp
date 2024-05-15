#include <RelayMqttSettingsService.h>

RelayMqttSettingsService::RelayMqttSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager) :
    _httpEndpoint(RelayMqttSettings::read,
                  RelayMqttSettings::update,
                  this,
                  server,
                  RELAY_BROKER_SETTINGS_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _fsPersistence(RelayMqttSettings::read, RelayMqttSettings::update, this, fs, RELAY_BROKER_SETTINGS_FILE) {
}

void RelayMqttSettingsService::begin() {
  _fsPersistence.readFromFS();
}
