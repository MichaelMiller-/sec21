import { useState } from 'react';

export interface ServiceInit {
    status: 'init';
}
export interface ServiceLoading {
    status: 'loading';
}
export interface ServiceLoaded<T> {
    status: 'loaded';
    payload: T;
}
export interface ServiceError {
    status: 'error';
    error: Error;
}
export type Service<T> =
    | ServiceInit
    | ServiceLoading
    | ServiceLoaded<T>
    | ServiceError;


export function fetchService<D>(url: string) 
{
    const [data, setData] = useState<Service<D> | null>(
    {
        status: 'loading'
    });        
    
    fetch(url)
        .then(response => {
            var res = response.json();
            return res;
        })
        .then(response => setData({ status: 'loaded', payload: response }))
        .catch(error => setData({ status: 'error', error }));
    
    return data;
}