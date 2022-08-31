import React from 'react';

import "./LandingPortal.css";
import PortalEntry from './PortalEntry';
import btBG from '../../assets/bluetooth-background.jpg';
import wifiBG from '../../assets/wifi-background.jpg';
import configBG from '../../assets/config-background.jpg';
import creditCardBG from '../../assets/credit-cards-background.jpeg';

function LandingPortal(props) {
  return (
    <div className="landing-portal-wrapper">
      <div className="title">
        <div>Configuration Portal</div>
        <div className="sub-desc">Please select a menu item to continue</div>
      </div>
      <div className="portal-entries-wrapper">
        <PortalEntry title="Credit Cards" location="financial-account" background={creditCardBG} />
        <PortalEntry title="General Config" location="config" background={configBG} />
        <PortalEntry title="Bluetooth" location="bluetooth" background={btBG} />
        <PortalEntry title="RaspberryPi WiFi" location="wifi" background={wifiBG} />
      </div>
    </div>
  )
}

export default LandingPortal;