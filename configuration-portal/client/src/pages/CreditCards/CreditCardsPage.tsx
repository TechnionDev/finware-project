import React, { useEffect, useState } from 'react';

const CREDIT_STATE_ENDPOINT = "/api/credit-cards";

function useCreditState() {
  const [creditState, setCreditState] = useState();
  useEffect(() => {
      fetch(CREDIT_STATE_ENDPOINT)
        .then((response) => { return response.json(); })
        .then((data) => { setCreditState(data); });
  }, []);
  return creditState;
}

function CreditCards() {
  const creditState = useCreditState();


  return (
    <div dangerouslySetInnerHTML={{__html: JSON.stringify(creditState || {}, null, 4).replaceAll("\n", "<br>")}}></div>
  );
}

export default CreditCards;