import React from 'react';
import {Offcanvas,} from "react-bootstrap";
import {LogEntryError, LogEntryWarning} from "./LogEntry";

type LogPaletteProps = {
    show: boolean
    onHide: () => void
    errorMessages: string[]
    warningMessages: string[]
}

const LogPalette = (props: LogPaletteProps) => {
    return (
        <Offcanvas show={props.show} onHide={() => (props.onHide())} placement={"start"}>
            <Offcanvas.Header closeButton>
                <Offcanvas.Title> </Offcanvas.Title>
            </Offcanvas.Header>
            <Offcanvas.Body>
                {props.errorMessages.map(e => (<LogEntryError message={e}/>))}
                {props.warningMessages.map(e => (<LogEntryWarning message={e}/>))}
            </Offcanvas.Body>
        </Offcanvas>
    )
}

export default LogPalette