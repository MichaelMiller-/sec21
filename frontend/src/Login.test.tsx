import React from 'react';
import { render, screen } from '@testing-library/react';
import Login from "./Login";

it('renders without crashing', async () => {
   render(<Login />);
   expect(screen.getByText('Token')).toBeInTheDocument();
   expect(screen.getByText('eMail Address')).toBeInTheDocument();
   expect(screen.getByRole('button')).toBeEnabled()
})
