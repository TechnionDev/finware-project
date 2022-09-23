import React, {useEffect} from 'react'
import "./WifiPage.css";

const WIFI_CONFIG_PORT = 8080;


function WifiPage() {
  useEffect(() => {
    document.location.href = `http://${document.location.hostname}:${WIFI_CONFIG_PORT}/`;
  }, []);
  return <></>;

  // const {protocol, hostname} = window.location;
  // return (
  //   <iframe title="wifi-frame" className="wifi-config" src={`${protocol}//${hostname}:${WIFI_CONFIG_PORT}`}></iframe>
  // )
}

export default WifiPage;