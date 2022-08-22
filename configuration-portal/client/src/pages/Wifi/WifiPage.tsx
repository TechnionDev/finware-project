import React from 'react'
import "./WifiPage.css";

const WIFI_CONFIG_PORT = 8080;


function WifiPage() {
  const {protocol, hostname} = window.location;
  return (
    <iframe className="wifi-config" src={`${protocol}//${hostname}:${WIFI_CONFIG_PORT}`}></iframe>
  )
}

export default WifiPage;