import { useState } from 'react';

const useSignedIn = () => {
    const [signedIn, setSignedIn] = useState(false);
    return { signedIn, setSignedIn };
}


const hooks = {
    useSignedIn: useSignedIn
}
export default hooks;