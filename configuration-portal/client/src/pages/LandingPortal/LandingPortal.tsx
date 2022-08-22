import React from 'react';

import "./LandingPortal.css";
import PortalEntry from './PortalEntry';
import btBG from '../../assets/bluetooth-background.jpg';
import wifiBG from '../../assets/wifi-background.jpg';
import configBG from '../../assets/config-background.jpg';

function LandingPortal(props) {
  return (
    <div className="landing-portal-wrapper">
      <div style={{ textAlign: "center", paddingTop: "16px", fontSize: "clamp(0px, 6vw, 30px)"}}>
        <div style={{ fontSize: "1em" }}>Configuration Portal</div>
        <div style={{ fontSize: "0.7em", color: "rgb(153 153 153)"}}>Please select a menu item to continue</div>
      </div>
      <div className="portal-entries-wrapper">
        <PortalEntry title="Bluetooth" location="bluetooth" background={btBG} />
        <PortalEntry title="Configuration" location="config" background={configBG} />
        <PortalEntry title="RaspberryPi WiFi" location="wifi" background={wifiBG} />
      </div>
      {props.children}
    </div>

  )
}

export default LandingPortal;