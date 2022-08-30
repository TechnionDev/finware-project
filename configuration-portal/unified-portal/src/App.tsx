import React from "react";
import { Routes, Route } from "react-router-dom";
import { Flowbite } from "flowbite-react";

import Theme from "./shared/theme.json";

import Navbar from './shared/components/Navbar';
import LandingPortal from './pages/landing-portal'
import ConfigPage from './pages/configuration'
import BluetoothPage from './pages/bluetooth'
import WifiPage from './pages/wifi'
import {
  FinanceAccountList,
  EditFinanceAccount
} from './pages/finance-accounts'


function App() {
  return (
    <div className="dark">
      <Flowbite theme={Theme}>
      </Flowbite>
      <Navbar></Navbar>
      <div className="content-wrapper flex justify-center">
        <Routes>
          <Route path="/">
            <Route index element={<LandingPortal></LandingPortal>} />
            <Route path="finance-accounts">
              <Route index element={<FinanceAccountList />} />
              <Route path="create" element={<EditFinanceAccount />} />
              <Route path="edit/:id" element={<EditFinanceAccount />} />
            </Route>
          </Route>
          <Route path="config" element={<ConfigPage />} />
          <Route path="bluetooth" element={<BluetoothPage />} />
          <Route path="wifi" element={<WifiPage />} />
        </Routes>
      </div>
    </div >

  );
}

export default App;

