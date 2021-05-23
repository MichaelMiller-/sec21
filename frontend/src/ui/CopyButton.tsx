import { Button } from "react-bootstrap";
import { faCopy } from "@fortawesome/free-solid-svg-icons"
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";

export type CopyButtonProps = {
    onClick: () => void;
    disabled: boolean;
}

export function CopyButton(props: CopyButtonProps) {
    return (
        <Button variant="link" onClick={props.onClick} disabled={props.disabled}>
            <FontAwesomeIcon icon={faCopy} size="2x" />
        </Button>
    )
}
