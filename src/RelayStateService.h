#ifndef RelayStateService_h
#define RelayStateService_h

#include <RelayMqttSettingsService.h>

#include <HttpEndpoint.h>
#include <MqttPubSub.h>
#include <WebSocketTxRx.h>

#define RELAY_PIN D1

#define DEFAULT_RELAY_STATE false
#define OFF_STATE "OFF"
#define ON_STATE "ON"

// Note that the built-in LED is on when the pin is low on most NodeMCU boards.
// This is because the anode is tied to VCC and the cathode to the GPIO 4 (Arduino pin 2).
#ifdef ESP32
#define RELAY_ON 0x1
#define RELAY_OFF 0x0
#elif defined(ESP8266)
#define RELAY_ON 0x0
#define RELAY_OFF 0x1
#endif

#define RELAY_SETTINGS_ENDPOINT_PATH "/rest/relayState"
#define RELAY_SETTINGS_SOCKET_PATH "/ws/relayState"

class RelayState {
 public:
  bool relay1On;

  static void read(RelayState& settings, JsonObject& root) {
    root["relay1_on"] = settings.relay1On;
  }

  static StateUpdateResult update(JsonObject& root, RelayState& relayState) {
    boolean newState = root["relay1_on"] | DEFAULT_RELAY_STATE;
    if (relayState.relay1On != newState) {
      relayState.relay1On = newState;
      return StateUpdateResult::CHANGED;
    }
    return StateUpdateResult::UNCHANGED;
  }

  static void haRead(RelayState& settings, JsonObject& root) {
    root["state"] = settings.relay1On ? ON_STATE : OFF_STATE;
  }

  static StateUpdateResult haUpdate(JsonObject& root, RelayState& relayState) {
    String state = root["state"];
    // parse new led state
    boolean newState = false;
    if (state.equals(ON_STATE)) {
      newState = true;
    } else if (!state.equals(OFF_STATE)) {
      return StateUpdateResult::ERROR;
    }
    // change the new state, if required
    if (relayState.relay1On != newState) {
      relayState.relay1On = newState;
      return StateUpdateResult::CHANGED;
    }
    return StateUpdateResult::UNCHANGED;
  }
};

class RelayStateService : public StatefulService<RelayState> {
 public:
  RelayStateService(AsyncWebServer* server,
                    SecurityManager* securityManager,
                    AsyncMqttClient* mqttClient,
                    RelayMqttSettingsService* relayMqttSettingsService);
  void begin();

 private:
  HttpEndpoint<RelayState> _httpEndpoint;
  MqttPubSub<RelayState> _mqttPubSub;
  WebSocketTxRx<RelayState> _webSocket;
  AsyncMqttClient* _mqttClient;
  RelayMqttSettingsService* _relayMqttSettingsService;

  void registerConfig();
  void onConfigUpdated();
};

#endif
