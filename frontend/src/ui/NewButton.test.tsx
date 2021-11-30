import React from 'react';
import { fireEvent, render, screen } from '@testing-library/react';
import NewButton from "./NewButton";

it('renders without crashing', async () => {
   render(<NewButton onClick={() => { }} disabled={false} title={"test123"}/>);
   expect(screen.getByRole('button')).toBeInTheDocument();
   expect(screen.getByRole('button')).not.toBeDisabled()
})

it('disable the button correctly', async () => {
   render(<NewButton onClick={() => { }} disabled={true} title={"test123"} />);
   expect(screen.getByRole('button')).toBeDisabled()
})

it('handles the click', async () => {
   let value = 0;
   render(<NewButton onClick={() => { value = 42; }} disabled={true} title={"test123"} />);

   fireEvent.click(screen.getByRole('button'))
   expect(value === 42);
})
