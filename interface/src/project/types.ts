export interface LightState {
  led_on: boolean;
}

export interface RelayState {
  relay1_on: boolean;
  relay2_on: boolean;
  relay3_on: boolean;
  relay4_on: boolean;
}

export interface LightMqttSettings {
  unique_id: string;
  name: string;
  mqtt_path: string;
}
