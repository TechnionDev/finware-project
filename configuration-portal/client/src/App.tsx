import React from "react";

import { Routes, Route } from "react-router-dom";
import LandingPortal from './pages/LandingPortal'
import ConfigPage from './pages/Configuration'
import BluetoothPage from './pages/Bluetooth'
import WifiPage from './pages/Wifi'


function App() {
  const [data, setData] = React.useState(null);

  React.useEffect(() => {
    fetch("/api")
      .then((res) => res.json())
      .then((data) => setData(data.message));
  }, []);

  return (
    <Routes>
      <Route path="/">
        <Route index element={<LandingPortal>{data}</LandingPortal>} />
        <Route path="bluetooth" element={<BluetoothPage />} />
        <Route path="config" element={<ConfigPage />} />
        <Route path="wifi" element={<WifiPage />} />
      </Route>
    </Routes>
  );
}

export default App;

