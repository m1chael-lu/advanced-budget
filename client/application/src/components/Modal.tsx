import React, { useState } from 'react';
import { Button, Modal, Box, TextField, Typography } from '@mui/material';
import { LocalizationProvider, DatePicker } from '@mui/x-date-pickers';
import { AdapterDateFns } from '@mui/x-date-pickers/AdapterDateFns';
import addExpense from '../api/addExpense';


interface FormData {
  name: string;
  description: string;
  startPeriod: Date | null;
  endPeriod: Date | null;
  value: number | '';
}

interface FormattedFormData {
    name: string;
    description: string;
    startPeriod: string;
    endPeriod: string;
    value: number;
}

const style = {
  position: 'absolute' as const,
  top: '50%',
  left: '50%',
  transform: 'translate(-50%, -50%)',
  width: 400,
  bgcolor: 'background.paper',
  border: '2px solid #000',
  boxShadow: 24,
  p: 4,
};

const MyModal: React.FC = () => {
  const [open, setOpen] = useState(false);
  const [formData, setFormData] = useState<FormData>({
    name: '',
    description: '',
    startPeriod: null,
    endPeriod: null,
    value: '',
  });

  const handleOpen = () => setOpen(true);
  const handleClose = () => setOpen(false);

  const handleInputChange = (event: React.ChangeEvent<HTMLInputElement>) => {
    setFormData({
      ...formData,
      [event.target.name]: event.target.value,
    });
  };

  const formatDates = (date: Date | null) => {
    if (date) {
        const pad = (num: number) => (num < 10 ? `0${num}` : num);

        const year = date.getFullYear();
        const month = pad(date.getMonth() + 1);  // Months are 0-based
        const day = pad(date.getDate());

        const hours = pad(date.getHours());
        const minutes = pad(date.getMinutes());
        const seconds = pad(date.getSeconds());

        return `${year}-${month}-${day} ${hours}:${minutes}:${seconds}`;
    }
    return '';
  }
  const handleDateChange = (name: keyof FormData) => (date: Date | null) => {
    setFormData({
      ...formData,
      [name]: date,
    });
  };

  const handleSubmit = (event: React.FormEvent<HTMLFormElement>) => {
    event.preventDefault();
    console.log(formData);
    const formattedFormData: FormattedFormData = {
        name: formData.name,
        description: formData.description,
        startPeriod: formatDates(formData.startPeriod) || '',
        endPeriod: formatDates(formData.endPeriod) || '',
        value: Number(formData.value),
    }
    console.log(formattedFormData);
    addExpense(formattedFormData);
    setOpen(false);
  };

  return (
    <div>
      <Button variant="contained" onClick={handleOpen}>Open Modal</Button>
      <Modal
        open={open}
        onClose={handleClose}
        aria-labelledby="modal-modal-title"
        aria-describedby="modal-modal-description"
      >
        <Box sx={style} component="form" onSubmit={handleSubmit} noValidate>
          <Typography id="modal-modal-title" variant="h6" component="h2">
            Add New Item
          </Typography>
          <TextField
            margin="normal"
            required
            fullWidth
            id="name"
            label="Name"
            name="name"
            autoComplete="name"
            autoFocus
            value={formData.name}
            onChange={handleInputChange}
          />
          <TextField
            margin="normal"
            required
            fullWidth
            name="description"
            label="Description"
            id="description"
            autoComplete="description"
            value={formData.description}
            onChange={handleInputChange}
          />
          <LocalizationProvider dateAdapter={AdapterDateFns}>
            <DatePicker
                label="Start Period"
                value={formData.startPeriod}
                onChange={handleDateChange('startPeriod')}
            />
            <DatePicker
                label="End Period"
                value={formData.endPeriod}
                onChange={handleDateChange('endPeriod')}
            />
            </LocalizationProvider>
          <TextField
            margin="normal"
            required
            fullWidth
            name="value"
            label="Value"
            type="number"
            id="value"
            autoComplete="value"
            value={formData.value}
            onChange={handleInputChange}
          />
          <Button type="submit" fullWidth variant="contained" sx={{ mt: 3, mb: 2 }}>
            Submit
          </Button>
        </Box>
      </Modal>
    </div>
  );
};

export default MyModal;
