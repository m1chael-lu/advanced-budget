import React, { useEffect, useState } from 'react';
import { format } from 'date-fns';
import {
  LineChart,
  Line,
  XAxis,
  YAxis,
  Tooltip,
  Legend,
  ResponsiveContainer,
  TooltipProps
} from 'recharts';


const CustomTooltip: React.FC<TooltipProps<number, string>> = ({ active, payload, label }) => {
    if (active && payload && payload.length > 0 && payload[0].value) {
      return (
        <div className="custom-tooltip" style={{ 
            backgroundColor: 'rgba(255, 255, 255, 0.8)', // 40% transparent white
            padding: '10px', 
            height: '80px',
            border: '1px solid #cccccc', 
            borderRadius: '10px',
            fontSize: '14px',
            color: 'black', // Ensuring text color is black for readability
            textShadow: '0px 0px 3px rgba(255, 255, 255, 0.9)', // Optional: text shadow for better readability
            fontFamily: 'sans-serif',
          }}>
          <p className="label">{`Date: ${label}`}</p>
          <p className="intro">{`Value: ${payload[0].value.toFixed(2)}`}</p>
        </div>
      );
    }
  
    return null;
  };


interface aggData {
  "day": string;
  "aggregate_value": number;
}
interface ChartProps {
  data: aggData[];
}

const Chart: React.FC<ChartProps> = ({ data }) => {
    console.log(data);
    return (
        <ResponsiveContainer width="100%" height={700}>
          <LineChart
            data={data}
            margin={{ top: 5, right: 30, left: 20, bottom: 5 }}
          >
            <defs>
              <linearGradient id="colorUv" x1="0" y1="0" x2="0" y2="1">
                <stop offset="5%" stopColor="#8884d8" stopOpacity={0.8}/>
                <stop offset="95%" stopColor="#8884d8" stopOpacity={0}/>
              </linearGradient>
            </defs>
            <XAxis 
                dataKey="day" 
                tickFormatter={(tickItem) => format(new Date(tickItem), 'MMM d')}
                style={{ fontSize: '12px' }} // Smaller font size
                />

                <YAxis style={{ fontSize: '12px' }} />
            <Tooltip content={<CustomTooltip />} />
            <Legend />
            <Line
              type="monotone"
              dataKey="aggregate_value"
              stroke="#8884d8"
              strokeWidth={2}
              dot={{ stroke: '#8884d8', strokeWidth: 2 }}
              activeDot={{ r: 8 }}
              fillOpacity={1}
              fill="url(#colorUv)"
            />
          </LineChart>
        </ResponsiveContainer>
      );
}

export default Chart;
