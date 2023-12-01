import { createSlice } from '@reduxjs/toolkit';

const userSlice = createSlice({
  name: 'userSlice',
  initialState: {
    id: 0,
  },
  reducers: {
    setID: (state, action) => {
        state.id = action.payload;
    }
  },
});

export const { setID } = userSlice.actions;

export default userSlice.reducer;
