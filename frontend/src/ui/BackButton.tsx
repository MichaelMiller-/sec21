import { Button } from "react-bootstrap";
import { faArrowCircleLeft } from "@fortawesome/free-solid-svg-icons"
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";

export type BackButtonProps = {
    onClick: () => void;
    disabled: boolean;
}

export function BackButton(props: BackButtonProps) {
    return (
        <Button variant="link" onClick={props.onClick} disabled={props.disabled}>
            <FontAwesomeIcon icon={faArrowCircleLeft} size="3x" />
        </Button>
    )
}
