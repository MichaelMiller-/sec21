import Button from "react-bootstrap/Button";
import { faArrowCircleLeft } from "@fortawesome/free-solid-svg-icons"
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";

export type BackButtonProps = {
   onClick: () => void;
   disabled: boolean;
}

const BackButton = (props: BackButtonProps) => {
   return (
      <Button variant="link" onClick={props.onClick} disabled={props.disabled}>
         <FontAwesomeIcon icon={faArrowCircleLeft} size="2x" />
      </Button>
   )
}

export default BackButton;
