import React, { useEffect, useState } from 'react';
import './App.css';
import { Provider } from 'react-redux';
import store from './state/store';
import AppWrapper from './components/AppWrapper';

export interface aggregateExpense {
  "day": string;
  "aggregate_value": number;
}

function App() {
  

  return (
    <div className="App App-Wrapper">
      <Provider store={store}>
        <div className='Expense-Title'>Advanced Expense Tracker</div> 
        <AppWrapper />
      </Provider>
    </div>
  );
}

export default App;
