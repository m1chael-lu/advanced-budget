import { configureStore } from '@reduxjs/toolkit';
import userReducer from './slices/user'
import dataReducer from './slices/data';  // Assuming the file name is dataSlice.js


export const store = configureStore({
    reducer: {
        user: userReducer,
        data: dataReducer,
    },
});

export default store;
