import { FC } from "react";

import { Switch } from "@mui/material";

import { WEB_SOCKET_ROOT } from "../api/endpoints";
import {
  BlockFormControlLabel,
  FormLoader,
  MessageBox,
  SectionContent,
} from "../components";
import { updateValue, useWs } from "../utils";

import { RelayState } from "./types";

export const RELAY_SETTINGS_WEBSOCKET_URL = WEB_SOCKET_ROOT + "relayState";

const HochbeetControl: FC = () => {
  const { connected, updateData, data } = useWs<RelayState>(
    RELAY_SETTINGS_WEBSOCKET_URL
  );

  const updateFormValue = updateValue(updateData);

  const content = () => {
    if (!connected || !data) {
      return <FormLoader message="Connecting to WebSocket…" />;
    }
    return (
      <>
        <MessageBox
          level="info"
          message="The switch below controls the Relay via the WebSocket. It will automatically update whenever the Relay state changes."
          my={2}
        />
        <BlockFormControlLabel
          control={
            <Switch
              name="relay1_on"
              checked={data.relay1_on}
              onChange={updateFormValue}
              color="primary"
            />
          }
          label="Relay1 State?"
        />
        <BlockFormControlLabel
          control={
            <Switch
              name="relay2_on"
              checked={data.relay2_on}
              onChange={updateFormValue}
              color="primary"
            />
          }
          label="Relay2 State?"
        />
        <BlockFormControlLabel
          control={
            <Switch
              name="relay3_on"
              checked={data.relay3_on}
              onChange={updateFormValue}
              color="primary"
            />
          }
          label="Relay3 State?"
        />
        <BlockFormControlLabel
          control={
            <Switch
              name="relay4_on"
              checked={data.relay4_on}
              onChange={updateFormValue}
              color="primary"
            />
          }
          label="Relay4 State?"
        />
      </>
    );
  };

  return (
    <SectionContent title="Hochbeet Control" titleGutter>
      {content()}
    </SectionContent>
  );
};

export default HochbeetControl;
