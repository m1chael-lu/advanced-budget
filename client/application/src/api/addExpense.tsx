import axios from 'axios';

interface BodyData {
    userid: number;
    name: string;
    description: string;
    startPeriod: string;
    endPeriod: string;
    value: number;
}

const addExpense = async (expense: any) => {
  const body: BodyData = {
    userid: 2,
    name: expense.name,
    description: expense.description,
    startPeriod: expense.startPeriod,
    endPeriod: expense.endPeriod,
    value: expense.value,
  }
  const response = await axios.post('http://localhost:8080/addexpense', body);
  axios.post('http://localhost:8080/addexpense', body)
  .then(response => {
    // Handle successful response
  })
  .catch(error => {
    if (error.response) {
      // The request was made and the server responded with a status code
      // that falls out of the range of 2xx
      console.error(error.response.data);
      console.error(error.response.status);
      console.error(error.response.headers);
    } else if (error.request) {
      // The request was made but no response was received
      console.error(error.request);
    } else {
      // Something happened in setting up the request that triggered an Error
      console.error('Error', error.message);
    }
  });
  return response.data;
};

export default addExpense;
