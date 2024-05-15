import { FC } from "react";
import { Navigate, Routes, Route } from "react-router-dom";

import DemoProject from "./DemoProject";
import Hochbeet from "./Hochbeet";

const ProjectRouting: FC = () => {
  return (
    <Routes>
      {
        // Add the default route for your project below
      }
      <Route path="/*" element={<Navigate to="hochbeet/socket" />} />
      {
        // Add your project page routes below.
      }
      <Route path="hochbeet/*" element={<Hochbeet />} />
      <Route path="demo/*" element={<DemoProject />} />
    </Routes>
  );
};

export default ProjectRouting;
