import Row from "react-bootstrap/Row";
import BackButton from "./BackButton";

export type HeaderProps = {
   onBack: () => void;
   disabledBackButton: boolean;
   title: string;
}

const Header = (props: HeaderProps) => {
   return (
      <>
         <Row>
            <BackButton disabled={props.disabledBackButton} onClick={props.onBack} />
            <h2>
               {props.title}</h2>
         </Row>
      </>
   )
}

export default Header;