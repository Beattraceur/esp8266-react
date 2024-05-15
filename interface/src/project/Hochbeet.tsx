import React, { FC } from "react";
import { Navigate, Route, Routes } from "react-router-dom";

import { Tab } from "@mui/material";

import { RouterTabs, useRouterTab, useLayoutTitle } from "../components";

import DemoInformation from "./DemoInformation";
import HochbeetControl from "./HochbeetControl";

const Hochbeet: FC = () => {
  useLayoutTitle("Hochbeet");
  const { routerTab } = useRouterTab();

  return (
    <>
      <RouterTabs value={routerTab}>
        <Tab value="information" label="Information" />

        <Tab value="socket" label="Control" />
      </RouterTabs>
      <Routes>
        <Route path="information" element={<DemoInformation />} />
        <Route path="socket" element={<HochbeetControl />} />
        <Route path="/*" element={<Navigate replace to="socket" />} />
      </Routes>
    </>
  );
};

export default Hochbeet;
