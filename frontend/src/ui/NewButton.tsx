import Button from "react-bootstrap/Button";
import { faPlusCircle } from "@fortawesome/free-solid-svg-icons"
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";

export type NewButtonProps = {
   onClick: () => void;
   disabled: boolean;
}

const NewButton = (props: NewButtonProps) => {
   return (
      <Button variant="link" onClick={props.onClick} disabled={props.disabled}>
         <FontAwesomeIcon icon={faPlusCircle} size="2x" />
      </Button>
   )
}

export default NewButton;