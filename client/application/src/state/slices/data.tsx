import { createSlice } from '@reduxjs/toolkit';

interface BodyData {
    userid: number;
    name: string;
    description: string;
    startPeriod: string;
    endPeriod: string;
    value: number;
}

interface AggregateData {
    aggregate_value: number;
    day: string;
}

const initialData: BodyData[] = [];
const initialAggregate: AggregateData[] = [];

const dataSlice = createSlice({
  name: 'userSlice',
  initialState: {
    data: initialData,
    aggregate_data: initialAggregate,
  },
  reducers: {
    setData: (state, action) => {
        state.data = action.payload;
    },
    setAggregateData: (state, action) => {
        state.aggregate_data = action.payload;
    },
    addData: (state, action) => {
        const body: BodyData = action.payload;
        state.data.push(body);
    }
  },
});

export const { setData, setAggregateData, addData } = dataSlice.actions;

export default dataSlice.reducer;
