import React from 'react';
import { fireEvent, render, screen } from '@testing-library/react';
import CopyButton from "./CopyButton";

it('renders without crashing', async () => {
   render(<CopyButton onClick={() => { }} disabled={false} />);
   expect(screen.getByRole('button')).toBeInTheDocument();
   expect(screen.getByRole('button')).not.toBeDisabled()
})

it('disable the button correctly', async () => {
   render(<CopyButton onClick={() => { }} disabled={true} />);
   expect(screen.getByRole('button')).toBeDisabled()
})

it('handles the click', async () => {
   let value = 0;
   render(<CopyButton onClick={() => { value = 42; }} disabled={true} />);

   fireEvent.click(screen.getByRole('button'))
   expect(value === 42);
})
