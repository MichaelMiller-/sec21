import {useState} from "react";
import {Alert} from "react-bootstrap";

export type DismissibleAlertProps = {
    message: string
}

const DismissibleAlert = (props: DismissibleAlertProps) => {
    const [show, setShow] = useState(true);

    if (props.message.length > 0 && show) {
        return (
            <Alert variant="danger" onClose={() => setShow(false)} dismissible>
                <Alert.Heading>Error</Alert.Heading>
                <p>
                    {props.message}
                </p>
            </Alert>
        );
    }
    return (<></>)
}

export default DismissibleAlert;
