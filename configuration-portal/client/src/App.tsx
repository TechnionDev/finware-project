import React from "react";
import { Routes, Route } from "react-router-dom";
import { Flowbite } from "flowbite-react";

import Theme from "./shared/theme.json";

import Navbar from './shared/components/Navbar';
import LandingPortal from './pages/LandingPortal'
import ConfigPage from './pages/Configuration'
import BluetoothPage from './pages/Bluetooth'
import WifiPage from './pages/Wifi'
import CreditCards from './pages/CreditCards'


function App() {
  return (
    <div className="dark">
      <Flowbite theme={Theme}>
      </Flowbite>
      <Navbar></Navbar>
      <div className="content-wrapper">
        <Routes>
          <Route path="/">
            <Route index element={<LandingPortal></LandingPortal>} />
            <Route path="credit-cards" element={<CreditCards />} />
            <Route path="config" element={<ConfigPage />} />
            <Route path="bluetooth" element={<BluetoothPage />} />
            <Route path="wifi" element={<WifiPage />} />
          </Route>
        </Routes>
      </div>
    </div>

  );
}

export default App;

