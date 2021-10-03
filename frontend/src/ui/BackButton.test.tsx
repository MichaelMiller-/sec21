import React from 'react';
import { render, screen } from '@testing-library/react';
import BackButton from "./BackButton";

it('renders without crashing', async () => {
   render(<BackButton onClick={() => {}} disabled={false} />);
   expect(screen.getByRole('button')).toBeInTheDocument();
})

//! \todo:
// - disable button
// - test click handler
