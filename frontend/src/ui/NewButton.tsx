import Button from "react-bootstrap/Button";
import { faPlusCircle } from "@fortawesome/free-solid-svg-icons"
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";

type NewButtonProps = {
   onClick: () => void
   disabled?: boolean
   title?: string
}

const NewButton = (props: NewButtonProps) => {
   return (
       <Button variant="link" onClick={props.onClick} disabled={props.disabled}>
             <FontAwesomeIcon icon={faPlusCircle} size="2x" onClick={props.onClick} />
       </Button>
   )
}

export default NewButton;