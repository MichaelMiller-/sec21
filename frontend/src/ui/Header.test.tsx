import React from 'react';
import { fireEvent, render, screen } from '@testing-library/react';
import Header from "./Header";

it('renders without crashing', async () => {
   render(<Header onBack={() => { }} disabledBackButton={false} title={'foobar'} />);

   expect(screen.getByRole('button')).toBeInTheDocument();
   expect(screen.getByRole('button')).not.toBeDisabled()
   expect(screen.getByRole('heading')).toHaveTextContent('foobar')
})

it('disable the back-button correctly', async () => {
   render(<Header onBack={() => { }} disabledBackButton={true} title={''} />);
   expect(screen.getByRole('button')).toBeDisabled()
})

it('handles the click', async () => {
   let value = 0;
   render(<Header onBack={() => { value = 42; }} disabledBackButton={false} title={''} />);

   fireEvent.click(screen.getByRole('button'))
   expect(value === 42);
})
