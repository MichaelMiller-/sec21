import { Button } from "react-bootstrap";
import { faEdit } from "@fortawesome/free-solid-svg-icons"
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";

export type EditButtonProps = {
    onClick: () => void;
    disabled: boolean;
}

export function EditButton(props: EditButtonProps) {
    return (
        <Button variant="link" onClick={props.onClick} disabled={props.disabled}>
            <FontAwesomeIcon icon={faEdit} size="2x" />
        </Button>
    )
}
