import { Button } from "react-bootstrap";
import { faTrash } from "@fortawesome/free-solid-svg-icons"
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";

export type RemoveButtonProps = {
    onClick: () => void;
    disabled: boolean;
}

export function RemoveButton(props: RemoveButtonProps) {
    return (
        <Button variant="link" onClick={props.onClick} disabled={props.disabled}>
            <FontAwesomeIcon icon={faTrash} size="2x" />
        </Button>
    )
}
