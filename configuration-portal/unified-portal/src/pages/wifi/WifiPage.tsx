import React from 'react'
import "./WifiPage.css";

const WIFI_CONFIG_PORT = 80;


function WifiPage() {
  const {protocol, hostname} = window.location;
  return (
    <iframe title="wifi-frame" className="wifi-config" src={`${protocol}//${hostname}:${WIFI_CONFIG_PORT}`}></iframe>
  )
}

export default WifiPage;