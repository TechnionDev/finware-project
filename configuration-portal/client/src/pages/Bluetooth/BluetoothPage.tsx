import React, { useEffect } from 'react';
import { useState } from 'react';
import './BluetoothPage.css';

const BT_STATE_ENDOINT = "/api/bt-state";
const POLLING_INTERVAL = 1000;
const DEFAULT_BT_STATE: BTState = {
  advertising: "IDLE"
}

interface BTState {
  advertising: "ACTIVE" | "IDLE"
};

function useBtState() {
  const [btState, setBtState] = useState<BTState>(DEFAULT_BT_STATE);

  useEffect(() => {
    const int = setInterval(() => {
      fetch(BT_STATE_ENDOINT)
        .then((response) => { return response.json(); })
        .then((data) => { setBtState(data); });
    }, POLLING_INTERVAL);

    return () => clearInterval(int);
  }, []);

  return btState;
}


function BluetoothPage() {
  const btState = useBtState();

  return (
    <div className="bt-wrapper">
      <div className="bt-inner-wrapper">
        <section className="bt-current-state">
          <p>{btState.advertising}</p>
        </section>
        <section>2</section>
        <section>3</section>
      </div>
    </div>
  );
}

export default BluetoothPage;