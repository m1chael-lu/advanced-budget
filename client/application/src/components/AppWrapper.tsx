import React, { useState, useEffect } from 'react';

import { useSelector, useDispatch } from 'react-redux';
import Chart from './Chart';
import MyTable from './Table';
import MyModal from './Modal';
import { setData, setAggregateData } from '../state/slices/data'

const fetch_data = async() => {
    const response = await fetch('http://localhost:8080/retrieveuserexpenses?id=2');
    const agg_response = await fetch('http://localhost:8080/aggregateexpenses?id=2');
    const agg_data = await agg_response.json(); 
    const data = await response.json();
    return [data.data, agg_data.data];
  }

const AppWrapper = () => {
    const dispatch = useDispatch();
    useEffect(() => {
        fetch_data().then((res) => {
          const [data, agg_data] = res;
          dispatch(setData(data));
          dispatch(setAggregateData(agg_data));
          console.log(res);
        })
      }, []);
    const data = useSelector((state: any) => state.data.data);
    const aggData = useSelector((state: any) => state.data.aggregate_data);
    const allData = useSelector((state: any) => state.data);
    console.log('all data');
    console.log(allData);
    console.log('normal data');
    console.log(data);
    console.log('aggdata');
    console.log(aggData);
    return (
        <div className="Chart-Wrapper">
          <Chart data={aggData}/>
          <MyTable rows={data}/>
          <MyModal />
        </div>   
    )
}

export default AppWrapper;
