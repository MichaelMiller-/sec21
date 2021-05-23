import { useState } from 'react';

export function useFetch<D>(url: string) {
   const [data, setData] = useState<D | null>(null);

   // Fetch the data here
   fetch(url)
      .then(res => {
         var ret = res.json();
         return ret;
      })
      .then(res => {
         console.log("res: ", res)
         setData(res);
      })
      .catch(err => {
         console.log("error", err)
      })
   return data;
}