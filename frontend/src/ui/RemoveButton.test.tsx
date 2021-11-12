import React from 'react';
import { fireEvent, render, screen } from '@testing-library/react';
import RemoveButton from "./RemoveButton";

it('renders without crashing', async () => {
   render(<RemoveButton onClick={() => { }} disabled={false} />);
   expect(screen.getByRole('button')).toBeInTheDocument();
   expect(screen.getByRole('button')).not.toBeDisabled()
})

it('disable the button correctly', async () => {
   render(<RemoveButton onClick={() => { }} disabled={true} />);
   expect(screen.getByRole('button')).toBeDisabled()
})

it('handles the click', async () => {
   let value = 0;
   render(<RemoveButton onClick={() => { value = 42; }} disabled={true} />);

   fireEvent.click(screen.getByRole('button'))
   expect(value === 42);
})
