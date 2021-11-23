import Button from "react-bootstrap/Button";
import { faFolderOpen } from "@fortawesome/free-solid-svg-icons"
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";

export type OpenButtonProps = {
   onClick: () => void;
   disabled?: boolean;
}

const OpenButton = (props: OpenButtonProps) => {
   return (
       <Button variant="link" onClick={props.onClick} disabled={props.disabled}>
          <FontAwesomeIcon icon={faFolderOpen} size="2x" onClick={props.onClick}/>
       </Button>
   )
}

export default OpenButton;