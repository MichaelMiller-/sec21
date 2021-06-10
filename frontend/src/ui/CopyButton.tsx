import Button from "react-bootstrap/Button";
import { faCopy } from "@fortawesome/free-solid-svg-icons"
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";

export type CopyButtonProps = {
   onClick: () => void;
   disabled: boolean;
}

const CopyButton = (props: CopyButtonProps) => {
   return (
      <Button variant="link" onClick={props.onClick} disabled={props.disabled}>
         <FontAwesomeIcon icon={faCopy} size="2x" />
      </Button>
   )
}

export default CopyButton;