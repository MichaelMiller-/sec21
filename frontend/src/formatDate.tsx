
// database cannot convert to typescript 'Date' class
const formatDate = (date: Date) : string => {
    let result = date.toString().replace('T', ' ');
    // cut seconds and milliseconds
    return result.slice(0, result.lastIndexOf(':'));
}

export default formatDate;