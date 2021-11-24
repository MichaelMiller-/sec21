import {Alert} from "react-bootstrap";
import {faBan, faExclamationTriangle} from "@fortawesome/free-solid-svg-icons";
import {FontAwesomeIcon} from "@fortawesome/react-fontawesome";
import React from "react";

type LogProps = {
    message: string
}

export const LogEntryError = (props: LogProps) => {
    return (
        <Alert variant="danger">
            <Alert.Heading>
                <FontAwesomeIcon icon={faBan} size="sm" />
            </Alert.Heading>
            <hr/>
            <p>{props.message}</p>
        </Alert>
    )
}

export const LogEntryWarning = (props: LogProps) => {
    return (
        <Alert variant="warning">
            <Alert.Heading>
                <FontAwesomeIcon icon={faExclamationTriangle} size="sm"/>
            </Alert.Heading>
            <hr/>
            <p>{props.message}</p>
        </Alert>
    )
}
